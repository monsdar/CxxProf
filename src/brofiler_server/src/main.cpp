
#include "brofiler_server/NetworkReceiver.h"
#include "brofiler_server/DataStorage.h"

#include "brofiler_dyn_network/ActivityResult.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

DataStorage storage_;

void receivedResult(const ActivityResult& result)
{
    std::cout << "Received result for " << result.Name << std::endl;
    storage_.storeResult(result);
}

int main()
{
    NetworkReceiver receiver;
    receiver.setCallback(receivedResult);
    receiver.startListening();

    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
