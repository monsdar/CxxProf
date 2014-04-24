
#include "cxxprof_server/NetworkReceiver.h"
#include "cxxprof_server/DataStorage.h"

#include "cxxprof_dyn_network/NetworkObjects.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

//Storage will create the session.db during initialization
//More control here would be better
DataStorage storage_;

void receivedObjects(const CxxProf::NetworkObjects& objects)
{
    std::cout << "Received " << objects.size() << " objects..." << std::endl;
    
    //NOTE: The outcommented code is a very basic way to profile the code.
    //      What about using CxxProf here? How do we need to set up everything for this to work?
    //boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
    storage_.storeResult(objects);
    //boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - start;
    //std::cout << "Storing took " << diff.total_milliseconds() << " milliseconds" << std::endl;
}

int main()
{
    //The receiver will connect itself to everything it needs
    //It uses default data to listenon localhost:15232
    NetworkReceiver receiver;
    receiver.setCallback(receivedObjects);
    receiver.startListening();
    std::cout << "Listening on tcp://*:15232..." << std::endl;

    //This is needed to keep the main-thread alive while the NetworkReceiver runs
    while(true)
    {
        boost::this_thread::sleep( boost::posix_time::milliseconds(200) );
    }

    return 0;
}
