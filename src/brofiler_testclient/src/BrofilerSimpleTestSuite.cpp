
#include "brofiler/Brofiler.h"

#define BOOST_TEST_MODULE BrofilerSimpleTest
#include <boost/test/unit_test.hpp>

#include <boost/shared_ptr.hpp>
#include <string>

BOOST_AUTO_TEST_CASE(simpleTest)
{
    Brofiler newBro;
    BOOST_CHECK_NO_THROW( newBro.printHello() );
}
