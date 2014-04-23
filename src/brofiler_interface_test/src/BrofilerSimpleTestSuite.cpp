
#include "brofiler_static/Brofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //Init the Brofiler first
    BROFILER_INIT();

    //Create a Mark
    BROFILER_MARK("TestMark");

    //Create a Plot
    BROFILER_PLOT("TestPlot", 1337.0);

    //Create an Activity
    BROFILER_ACTIVITY("TestActivity1");
    {
        BROFILER_ACTIVITY("TestActivity2");
    }
    BROFILER_ACTIVITY("TestActivity3");

    //Shutdown the Brofiler cleanly
    BROFILER_SHUTDOWN();
}
