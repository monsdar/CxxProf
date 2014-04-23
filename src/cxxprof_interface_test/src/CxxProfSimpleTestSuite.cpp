
#include "cxxprof_static/CxxProf.h"

#define BOOST_TEST_MODULE CxxProfSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //Init the CxxProf first
    CXXPROF_INIT();

    //Create a Mark
    CXXPROF_MARK("TestMark");

    //Create a Plot
    CXXPROF_PLOT("TestPlot", 1337.0);

    //Create an Activity
    CXXPROF_ACTIVITY("TestActivity1");
    {
        CXXPROF_ACTIVITY("TestActivity2");
    }
    CXXPROF_ACTIVITY("TestActivity3");

    //Shutdown the CxxProf cleanly
    CXXPROF_SHUTDOWN();
}
