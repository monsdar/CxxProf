
#include "cxxprof_server/NetworkReceiver.h"

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

void NetworkReceiver::setCallback(ReceiveCallback givenCallback)
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
            //TODO: During deserialization we're not receiving any new data.
            //      As this could take some time we should think about serializing in an
            //      explicit thread
            std::stringstream dataStream;
            dataStream << data;
            boost::archive::text_iarchive ia(dataStream);
            CxxProf::NetworkObjects objects;
            ia >> objects;

            if (callback_)
            {
                //TODO: The call to the callback is blocking, it could take a lot of time to
                //      perform it. In that time we cannot receive new data.
                //      Do we need a system here which prevents this?
                callback_(objects);
            }
        }

        //let's give this thread a break every now and then... CPU will thank us ;)
        //Sleep is the easiest interruption-point here (see Boost docs for other types of interruption points)
        boost::this_thread::sleep( boost::posix_time::milliseconds(0) );
    }
}
