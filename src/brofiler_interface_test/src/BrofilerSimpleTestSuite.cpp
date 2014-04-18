
#include "brofiler_static/Brofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //NOTE: We had a lot more in this test in the beginning of the project... currently it's kind of useless to be honest :/

    //let's try to create an Activity
    std::cout << "Activity focus test..." << std::endl;
    {
        BROFILER_ACTIVITY("TestActivity");
    }
    std::cout << "...Activity focus test ended" << std::endl;
}
