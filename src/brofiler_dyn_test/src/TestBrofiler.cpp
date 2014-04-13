
#include "brofiler_dyn_test/TestBrofiler.h"
#include "brofiler_dyn_test/TestActivity.h"

TestBrofiler::TestBrofiler()
{}

TestBrofiler::~TestBrofiler()
{}

boost::shared_ptr<IActivity> TestBrofiler::createActivity(const std::string& name)
{
    return boost::shared_ptr<IActivity>( new TestActivity(name) );
}

std::string TestBrofiler::toString() const
{
    return "TestBrofiler";
}
