
#include "brofiler/Brofiler.h"
#include "brofiler_dyntest/TestBrofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    //Create the Brofiler
    std::cout << "Creating Brofiler" << std::endl;
    boost::shared_ptr<Brofiler> newBro( new Brofiler() );
    
    //The brofiler should return NULL activity if there is no DynBrofiler loaded
    std::cout << "Trying to create Activity without DynBrofiler" << std::endl;
    boost::shared_ptr<IActivity> nullAct;
    BOOST_CHECK_NO_THROW( nullAct = newBro->createActivity("FailActivity") );
    BOOST_CHECK( nullAct.get() == NULL ); //Activity should be NULL
    
    //Load a DynBrofiler
    std::cout << "Loading DynBrofiler" << std::endl;
    BOOST_CHECK_NO_THROW( newBro->loadDynBrofiler() );
    
    //let's try to create an Activity
    std::cout << "Activity focus test..." << std::endl;
    {
        boost::shared_ptr<IActivity> act;
        std::cout << "Creating Activity" << std::endl;
        BOOST_CHECK_NO_THROW( act = newBro->createActivity("MyFirstActivity") );
        BOOST_CHECK( act.get() != NULL ); //Activity should NOT be NULL
    }
    std::cout << "...Activity focus test ended" << std::endl;
}
