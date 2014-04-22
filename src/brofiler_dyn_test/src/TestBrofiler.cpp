
#include "brofiler_dyn_test/TestBrofiler.h"
#include "brofiler_dyn_test/TestActivity.h"

#include <iostream>

TestBrofiler::TestBrofiler()
{}

TestBrofiler::~TestBrofiler()
{}

boost::shared_ptr<IActivity> TestBrofiler::createActivity(const std::string& name)
{
    return boost::shared_ptr<IActivity>( new TestActivity(name) );
}
void TestBrofiler::addMark(const std::string& name)
{
    std::cout << "Adding mark " << name << std::endl;
}
void TestBrofiler::addPlotValue(const std::string& name, double value)
{
    std::cout << "Adding plot value " << name << " with value " << value << std::endl;
}
void TestBrofiler::shutdown()
{
    std::cout << "Shutdown called" << std::endl;
}

std::string TestBrofiler::toString() const
{
    return "TestBrofiler";
}
