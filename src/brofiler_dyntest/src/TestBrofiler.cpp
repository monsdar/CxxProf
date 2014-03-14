
#include "brofiler_dyntest/TestBrofiler.h"
#include "brofiler_dyntest/TestActivity.h"

TestBrofiler::TestBrofiler()
{}

TestBrofiler::~TestBrofiler()
{}

boost::shared_ptr<IActivity> TestBrofiler::createActivity(const std::string& name) const
{
    return boost::shared_ptr<IActivity>( new TestActivity(name) );
}
