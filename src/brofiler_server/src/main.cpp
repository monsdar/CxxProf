
#include "brofiler_dyn/ActivityResult.h"
#include "brofiler_dyn/Serializers.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <zhelpers.hpp>

#include <iostream>
#include <sstream>

zmq::context_t zmqContext_(1);
zmq::socket_t zmqListener_(zmqContext_, ZMQ_PULL);

void receive()
{
    std::string data;
    while (data.empty())
    {
        data = s_recv(zmqListener_);
        if (data.size() == 0)
        {
            std::cout << "Received empty message" << std::endl;
        }
        else
        {
            std::cout << "Received: " << data << std::endl;
        }
    }

    std::cout << "Deserializing Data..." << std::endl;
    std::stringstream dataStream;
    dataStream << data;
    boost::archive::text_iarchive ia(dataStream);
    ActivityResult result;
    ia >> result;
    std::cout << "Received result for " << result.Name << std::endl;
}

int main()
{
    std::cout << "Starting Server" << std::endl;
    zmqListener_.bind("tcp://*:15232");

    std::cout << "Listening..." << std::endl;
    boost::thread listenThread(receive);

    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
