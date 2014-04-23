

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "brofiler_static/Brofiler.h"

const unsigned int NUM_THREADS = 5;

void longOperation()
{
    BROFILER_ACTIVITY("longOperation");
    boost::this_thread::sleep(boost::posix_time::seconds(1));
}

void loopOperation(unsigned int numLoops)
{
    BROFILER_ACTIVITY("loopOperation");
    for (unsigned int index = 0; index < numLoops; ++index)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
    }
}

int recursiveOperation(int someValue)
{
    BROFILER_ACTIVITY("recursiveOperation");

    //Adding PlotValues helps us to see how the application is handling the change of values over time
    BROFILER_PLOT("SomeValue", someValue);

    if (someValue == 3)
    {
        //Let's add some complexity by sleeping a bit when someValue reaches 3
        BROFILER_ACTIVITY("recursiveOperation::middle");
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
    }
    if(someValue < 5)
    {
        someValue = recursiveOperation(++someValue);
    }

    return someValue;
}

int main()
{
    //Initialize Brofiler first
    BROFILER_INIT();

    //Measure how long the rest of this main() takes
    BROFILER_ACTIVITY("main");
    
    //Add a mark for a better overview of which state the application is currently in
    BROFILER_MARK("LongOperation start");
    longOperation();
    longOperation();

    //Start some LoopOperations in extra Threads
    BROFILER_MARK("LoopThread start");
    std::vector< boost::shared_ptr<boost::thread> > threads;
    for (unsigned int index = 0; index < NUM_THREADS; ++index)
    {
        boost::shared_ptr<boost::thread> loopThread( new boost::thread(boost::bind(loopOperation, 5)) );
        threads.push_back(loopThread);
    }

    //And another mark
    BROFILER_MARK("RecursiveOperation start");
    recursiveOperation(1);

    //wait until the loopThreads are finished
    std::vector<boost::shared_ptr<boost::thread> >::iterator threadIter = threads.begin();
    for (; threadIter != threads.end(); ++threadIter)
    {
        (*threadIter)->join();
    }

    //See that it's possible to create multiple Activities with the same name in the same scope
    BROFILER_MARK("ScopeTest start");
    for (unsigned int index = 0; index < 5; index++)
    {
        BROFILER_ACTIVITY("dtorTest");
        BROFILER_ACTIVITY("dtorTest");
    }

    //Shutdown the Brofiler cleanly
    BROFILER_SHUTDOWN();

    return 0;
}
