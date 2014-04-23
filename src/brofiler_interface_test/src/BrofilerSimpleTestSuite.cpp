
#include "brofiler_static/Brofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //Create a Mark
    std::cout << "Mark test..." << std::endl;
    BROFILER_MARK("TestMark");
    std::cout << "...Mark test ended" << std::endl;

    //Create a Plot
    std::cout << "Plot test..." << std::endl;
    BROFILER_PLOT("TestPlot", 1337.0);
    std::cout << "...Plot test ended" << std::endl;

    //Create an Activity
    std::cout << "Activity test..." << std::endl;
    BROFILER_ACTIVITY("TestActivity1");
    {
        BROFILER_ACTIVITY("TestActivity2");
    }
    BROFILER_ACTIVITY("TestActivity3");
    std::cout << "...Activity test ended" << std::endl;

    //Shutdown the Brofiler cleanly
    BROFILER_SHUTDOWN();
}
