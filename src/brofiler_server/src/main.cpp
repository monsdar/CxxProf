
#include "brofiler_server/NetworkReceiver.h"
#include "brofiler_server/DataStorage.h"

#include "brofiler_dyn_network/NetworkObjects.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

DataStorage storage_;

void receivedObjects(const NetworkObjects& objects)
{
    std::cout << "Received objects..." << std::endl;
    storage_.storeResult(objects);
}

int main()
{
    NetworkReceiver receiver;
    receiver.setCallback(receivedObjects);
    receiver.startListening();

    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
