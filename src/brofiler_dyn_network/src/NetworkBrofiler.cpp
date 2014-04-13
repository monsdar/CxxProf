
#include "brofiler_dyn_network/NetworkBrofiler.h"
#include "brofiler_dyn_network/NetworkActivity.h"
#include "brofiler_dyn_network/Serializers.h"

#include <boost/bind.hpp>

//NOTE: We need to use the C-API of ZMQ here because Pluma and WinSock2.h both implement a connect() function
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

    boost::shared_ptr<NetworkActivity> newAct(new NetworkActivity(  name, 
                                                                    newActId, 
                                                                    0, //TODO: Get a Thread-ID
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

void NetworkBrofiler::addResult(const ActivityResult& result)
{
    activeActivity_.pop();

    //serialize the result
    std::ostringstream serializeStream;
    boost::archive::text_oarchive oa(serializeStream);
    oa << result;

    //send the activity data via network
    std::string serializeString = serializeStream.str();
    zmq_send(zmqSender_, serializeString.c_str(), serializeString.size(), 0);
}

std::string NetworkBrofiler::toString() const
{
    return "NetworkBrofiler";
}
