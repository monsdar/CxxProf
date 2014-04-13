
#include "brofiler_server/NetworkReceiver.h"
#include "brofiler_dyn_network/ActivityResult.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <sqlite3cc.h>

#include <iostream>
#include <sstream>


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

void receivedResult(const ActivityResult& result)
{
    std::cout << "Received " << result.Name << std::endl;
}

int main()
{
    //checkSqliteCode();

    NetworkReceiver receiver;
    receiver.setCallback(receivedResult);
    receiver.startListening();

    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
