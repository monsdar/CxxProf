
#include "brofiler_dyn_network/ActivityResult.h"
#include "brofiler_dyn_network/Serializers.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <sqlite3cc.h>
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

void checkSqliteCode()
{
    sqlite::connection conn("data.s3db");
    sqlite::query queryAllActivities(conn, "SELECT * FROM activities");
    std::cout << "Results: " << queryAllActivities.num_results() << std::endl;

    for (sqlite::query::iterator iter = queryAllActivities.begin(); iter != queryAllActivities.end(); iter++)
    {
        unsigned int actId = iter->column< unsigned int >(0);
        unsigned int threadId = iter->column< unsigned int >(1);
        unsigned int parentId = iter->column< unsigned int >(2);
        unsigned int startTime = iter->column< unsigned int >(3);
        unsigned int stopTime = iter->column< unsigned int >(4);
        std::string name = iter->column< std::string >(5);

        std::cout << std::endl;
        std::cout << "Row Number: " << iter->row_number() << std::endl;
        std::cout << "Activity " << name << " (" << actId << ")" << std::endl;
        std::cout << "Thread: " << threadId << std::endl;
        std::cout << "Parent: " << parentId << std::endl;
        std::cout << "Start: " << startTime << std::endl;
        std::cout << "Stop: " << stopTime << std::endl;
    }
}

int main()
{
    std::cout << "Starting Server" << std::endl;
    zmqListener_.bind("tcp://*:15232");

    checkSqliteCode();

    std::cout << "Listening..." << std::endl;
    boost::thread listenThread(receive);

    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
