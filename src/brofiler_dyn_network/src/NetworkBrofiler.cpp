
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
    zmqSender_(zmq_socket(zmqContext_, ZMQ_PUSH))
{
    zmq_connect(zmqSender_, "tcp://localhost:15232");
}

NetworkBrofiler::~NetworkBrofiler()
{
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
    
    return newAct;
}

void NetworkBrofiler::addMark(const std::string& name)
{
    //Build the NetworkMark
    NetworkMark newMark;
    newMark.Name = name;
    newMark.Timestamp = (boost::posix_time::microsec_clock::local_time() - profilingStart_).total_milliseconds();
    sendObjects_.Marks.push_back(newMark);

    //Send the objects
    //TODO: Do not send as soon as something new is in. Send in longer intervals timed by an additional thread or something like that.
    sendObjects();
}
void NetworkBrofiler::addPlotValue(const std::string& name, double value)
{
    //Build the NetworkMark
    NetworkPlot newPlot;
    newPlot.Name = name;
    newPlot.Timestamp = (boost::posix_time::microsec_clock::local_time() - profilingStart_).total_milliseconds();
    newPlot.Value = value;
    sendObjects_.Plots.push_back(newPlot);

    //Send the objects
    //TODO: Do not send as soon as something new is in. Send in longer intervals timed by an additional thread or something like that.
    sendObjects();
}
void NetworkBrofiler::addResult(const ActivityResult& result)
{
    //the mutex protects from different activities calling this callback at once
    boost::mutex::scoped_lock lock(callbackMutex_);

    activeActivity_.pop();
    sendObjects_.ActivityResults.push_back(result);

    //Send the objects
    //TODO: Do not send as soon as something new is in. Send in longer intervals timed by an additional thread or something like that.
    sendObjects();
}

void NetworkBrofiler::sendObjects()
{    
    //serialize the result
    std::ostringstream serializeStream;
    boost::archive::text_oarchive oa(serializeStream);
    oa << sendObjects_;

    //send the activity data via network
    std::string serializeString = serializeStream.str();
    zmq_send(zmqSender_, serializeString.c_str(), serializeString.size(), 0);

    sendObjects_.clear();
}

std::string NetworkBrofiler::toString() const
{
    return "NetworkBrofiler";
}
