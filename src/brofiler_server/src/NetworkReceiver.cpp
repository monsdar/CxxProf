
#include "brofiler_server/NetworkReceiver.h"

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <string>

NetworkReceiver::NetworkReceiver() :
    zmqContext_(new zmq::context_t(1)),
    zmqListener_(new zmq::socket_t(*zmqContext_, ZMQ_PULL)),
    isRunning_(false)
{
    zmqListener_->bind("tcp://*:15232");
}

NetworkReceiver::~NetworkReceiver()
{
    isRunning_ = false;
}

void NetworkReceiver::setCallback(ResultCallback givenCallback)
{
    callback_ = givenCallback;
}

void NetworkReceiver::startListening()
{
    //do not start listening twice
    if (isRunning_)
    {
        return;
    }

    isRunning_ = true;
    listenThread_.reset( new boost::thread(boost::bind(&NetworkReceiver::receive, this)) );
}

void NetworkReceiver::receive()
{
    while (isRunning_)
    {
        std::string data = s_recv(*zmqListener_);
        if (data.size() != 0)
        {
            std::stringstream dataStream;
            dataStream << data;
            boost::archive::text_iarchive ia(dataStream);
            ActivityResult result;
            ia >> result;

            if (callback_)
            {
                callback_(result);
            }
        }

        //let's give this thread a break every now and then... CPU will thank us ;)
        boost::this_thread::sleep( boost::posix_time::milliseconds(0) );
    }
}