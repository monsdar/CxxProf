
#include "brofiler/Brofiler.h"
#include "brofiler_dyntest/TestBrofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //create the Brofiler along with its DynBrofiler
    boost::shared_ptr<IDynBrofiler> dynBrofiler( new TestBrofiler() );
    Brofiler newBro(dynBrofiler);
    
    newBro.loadDynBrofiler();
    
    //let's try to create an Activity
    std::cout << "Activity gets created..." << std::endl;
    {
        boost::shared_ptr<IActivity> act;
        BOOST_CHECK_NO_THROW( act = newBro.createActivity("MyFirstActivity") );
    }
    std::cout << "Activity should be destructed by now..." << std::endl;
}
