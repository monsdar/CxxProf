
#include "cxxprof_static/CxxProf.h"

#define BOOST_TEST_MODULE CxxProfSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

//This is a test which simply calls all of the CxxProf macros in their respective order...
//Nothing special to see here... There's also nothing really to Unittest here :/

//TODO: Perhaps we could integrate a mocking framework instead of the dyn_test plugin.
//      Another interesting thing would be to do some integration tests with dyn_test and a mock-server
//      or something like that.
BOOST_AUTO_TEST_CASE(simpleTest)
{
    //Set the alias for this process
    CXXPROF_PROCESS_ALIAS("cxxprof_interface_test");

    //Set the alias for this thread
    CXXPROF_THREAD_ALIAS("MainThread");

    //Init the CxxProf first
    CXXPROF_INIT();

    //Create a Mark
    CXXPROF_MARK("TestMark");

    //Create a Plot
    CXXPROF_PLOT("TestPlot", 1337.0);

    //Create an Activity
    CXXPROF_ACTIVITY("TestActivity1");
    {
        //This activity should end before TestActivity1 and TestActivity3...
        CXXPROF_ACTIVITY("TestActivity2");
    }
    //just to see if it works that there are multiple activities created in the same scope...
    CXXPROF_ACTIVITY("TestActivity3");

    //Shutdown the CxxProf cleanly
    CXXPROF_SHUTDOWN();
}
