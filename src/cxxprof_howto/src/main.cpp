

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "cxxprof_static/CxxProf.h"

//This is for the Threading-Test, more then 5 seem to clutter the resulting data
const unsigned int NUM_THREADS = 5;

/**
 * This is simply a method which takes a lot of time and therefore should
 * produce a very long activity
 */
void longOperation(double givenTime)
{
    CXXPROF_ACTIVITY("longOperation");
    boost::this_thread::sleep(boost::posix_time::milliseconds(givenTime));
}

int recursiveOperation(int someValue)
{
    CXXPROF_ACTIVITY("recursiveOperation");

    //Adding PlotValues helps us to see how the application is handling the change of values over time
    CXXPROF_PLOT("SomeValue", someValue);

    if (someValue == 3)
    {
        //Let's add some complexity by sleeping a bit when someValue reaches 3
        CXXPROF_ACTIVITY("recursiveOperation::middle");
        longOperation(200.0);        
    }
    if(someValue < 5) //do recursion until the value hits 5
    {
        //just sleep a bit to simulate something costly going on here...
        longOperation(50.0);
        someValue = recursiveOperation(++someValue);
    }

    return someValue;
}

int main()
{
    //Initialize CxxProf first
    CXXPROF_INIT();

    //Measure how long the rest of this main() takes
    CXXPROF_ACTIVITY("main");
    
    //Add a mark for a better overview of which state the application is currently in
    CXXPROF_MARK("LongOperation start");
    longOperation(1000.0);
    longOperation(800.0);

    //Start some LoopOperations in extra Threads
    CXXPROF_MARK("Threading start");
    std::vector< boost::shared_ptr<boost::thread> > threads;
    for (unsigned int index = 0; index < NUM_THREADS; ++index)
    {
        //create a new thread which waits 100 millisecs
        boost::shared_ptr<boost::thread> newThread( new boost::thread(boost::bind(longOperation, 100.0)) );
        threads.push_back(newThread);
    }

    //As long as the threads are running let's do the recursive test
    //This should result in some activities which are stacked upon each other
    CXXPROF_MARK("RecursiveOperation start");
    recursiveOperation(1);

    //Now let's wait until the loopThreads are finished
    std::vector<boost::shared_ptr<boost::thread> >::iterator threadIter = threads.begin();
    for (; threadIter != threads.end(); ++threadIter)
    {
        (*threadIter)->join();
    }

    //Here it is demonstrated that we can also create multiple Activities in the same scope
    CXXPROF_MARK("ScopeTest start");
    {
        CXXPROF_ACTIVITY("scopeTest");
        CXXPROF_ACTIVITY("scopeTest");
    }

    //Shutdown the CxxProf cleanly
    //This ends the 'main' activity and everything else which is still running (probably the scopeTest)
    //It also takes care of sending the data which hasn't been sent yet
    CXXPROF_SHUTDOWN();

    return 0;
}
