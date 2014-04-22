
#include "brofiler_dyn_network/NetworkBrofiler.h"
#include "brofiler_dyn_network/NetworkActivity.h"
#include "brofiler_dyn_network/NetworkMark.h"
#include "brofiler_dyn_network/NetworkPlot.h"
#include "brofiler_dyn_network/Serializers.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

//NOTE: We need to use the C-API of ZMQ here because Pluma and WinSock2.h both implement a connect() function.
//      Due to extern C in the WinSock part it is not possible to have both defined in the global space -.-
//
//Error Message when using zhelpers.hpp here:
//      C2733: 'connect': second C linkage of overloaded function not allowed
//
#include <zhelpers.h>

#include <iostream>
#include <sstream>

NetworkBrofiler::NetworkBrofiler() :
    actCounter_(1),
    profilingStart_(boost::posix_time::microsec_clock::local_time()),
    zmqContext_(zmq_ctx_new()),
    zmqSender_(zmq_socket(zmqContext_, ZMQ_PUSH)),
    isSending_(false)
{
    zmq_connect(zmqSender_, "tcp://localhost:15232");

    sendThread_ = boost::thread(boost::bind(&NetworkBrofiler::sendLoop, this));
}

NetworkBrofiler::~NetworkBrofiler()
{
    isSending_ = false;
    zmq_close(zmqSender_);
    zmq_ctx_destroy(zmqContext_);
}

boost::shared_ptr<IActivity> NetworkBrofiler::createActivity(const std::string& name)
{
    unsigned int newActId = actCounter_++;

    //get the current Thread Id
    unsigned int threadId = 0;
    std::vector<boost::thread::id>::iterator resultIter = std::find(knownThreads_.begin(), knownThreads_.end(), boost::this_thread::get_id());
    if (resultIter == knownThreads_.end())
    {
        knownThreads_.push_back(boost::this_thread::get_id());
        threadId = knownThreads_.size() - 1;
    }
    else
    {
        //NOTE: See http://stackoverflow.com/questions/2152986/best-way-to-get-the-index-of-an-iterator
        //      on why we do not use std::distance here
        threadId = resultIter - knownThreads_.begin();
    }

    boost::shared_ptr<NetworkActivity> newAct(new NetworkActivity(  name, 
                                                                    newActId, 
                                                                    threadId,
                                                                    0, //we're setting the parentId later
                                                                    profilingStart_,
                                                                    boost::bind(&NetworkBrofiler::addResult, this, _1)));
    
    //check if this is the newest Activity
    //set the ParentId then
    if (activeActivity_.empty())
    {
        newAct->setParentId(0);
    }
    else
    {
        newAct->setParentId(activeActivity_.top());
    }

    //make the new activity the active one
    newAct->start();
    activeActivity_.push(newActId);

    boost::weak_ptr<NetworkActivity> weakAct(newAct);
    activities_[newActId] = weakAct;
    
    return newAct;
}

void NetworkBrofiler::addMark(const std::string& name)
{
    //Build the NetworkMark
    NetworkMark newMark;
    newMark.Name = name;
    newMark.Timestamp = (boost::posix_time::microsec_clock::local_time() - profilingStart_).total_microseconds();

    //Protect the SendObjects_
    boost::mutex::scoped_lock lock(sendMutex_);
    sendObjects_.Marks.push_back(newMark);
}
void NetworkBrofiler::addPlotValue(const std::string& name, double value)
{
    //Build the NetworkMark
    NetworkPlot newPlot;
    newPlot.Name = name;
    newPlot.Timestamp = (boost::posix_time::microsec_clock::local_time() - profilingStart_).total_microseconds();
    newPlot.Value = value;

    //Protect the SendObjects_
    boost::mutex::scoped_lock lock(sendMutex_);
    sendObjects_.Plots.push_back(newPlot);
}
void NetworkBrofiler::addResult(const ActivityResult& result)
{
    //the mutex protects from different activities calling this callback at once
    boost::mutex::scoped_lock callbackLock(callbackMutex_);

    activeActivity_.pop();
    activities_.erase(result.ActId);
    
    //Protect the SendObjects_
    boost::mutex::scoped_lock sendLock(sendMutex_);
    sendObjects_.ActivityResults.push_back(result);
}

void NetworkBrofiler::shutdown()
{
    //shutdown all active Activities
    std::map<unsigned int, boost::weak_ptr<NetworkActivity> >::iterator actIter = activities_.begin();
    for (; actIter != activities_.end(); ++actIter)
    {
        boost::shared_ptr<NetworkActivity> lockerAct = actIter->second.lock();
        lockerAct->shutdown();
    }

    //Send the objects
    sendObjects();
}

void NetworkBrofiler::sendLoop()
{
    isSending_ = true;
    while (isSending_)
    {
        //sleep a bit and wait for more objects
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

        //Protect the SendObjects_
        {
            boost::mutex::scoped_lock lock(sendMutex_);

            //let's just check if there is anything to send...
            if (sendObjects_.size() == 0)
            {
                continue;
            }
        }

        //send everything we have
        sendObjects();
    }
}

void NetworkBrofiler::sendObjects()
{
    //serialize the result
    std::ostringstream serializeStream;

    //Protect the SendObjects_
    {
        boost::mutex::scoped_lock lock(sendMutex_);

        boost::archive::text_oarchive oa(serializeStream);
        oa << sendObjects_;
        sendObjects_.clear();
    }

    //send the activity data via network
    std::string serializeString = serializeStream.str();
    zmq_send(zmqSender_, serializeString.c_str(), serializeString.size(), 0);
}

std::string NetworkBrofiler::toString() const
{
    return "NetworkBrofiler";
}
