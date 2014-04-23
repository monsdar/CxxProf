
#include "cxxprof_server/NetworkReceiver.h"
#include "cxxprof_server/DataStorage.h"

#include "cxxprof_dyn_network/NetworkObjects.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

DataStorage storage_;

void receivedObjects(const CxxProf::NetworkObjects& objects)
{
    //std::cout << "Received " << objects.size() << " objects..." << std::endl;
    //boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
    storage_.storeResult(objects);
    //boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - start;
    //std::cout << "Storing took " << diff.total_milliseconds() << " milliseconds" << std::endl;
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
