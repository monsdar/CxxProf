
#include "cxxprof_dyn_test/TestCxxProf.h"
#include "cxxprof_dyn_test/TestActivity.h"

#include <iostream>

namespace CxxProf
{

    TestCxxProf::TestCxxProf()
    {}

    TestCxxProf::~TestCxxProf()
    {}

    boost::shared_ptr<IActivity> TestCxxProf::createActivity(const std::string& name)
    {
        return boost::shared_ptr<IActivity>(new TestActivity(name));
    }
    void TestCxxProf::addMark(const std::string& name)
    {
        std::cout << "Adding mark " << name << std::endl;
    }
    void TestCxxProf::addPlotValue(const std::string& name, double value)
    {
        std::cout << "Adding plot value " << name << " with value " << value << std::endl;
    }
    void TestCxxProf::setProcessAlias(const std::string name)
    {
        std::cout << "New process alias: " << name << std::endl;
    }
    void TestCxxProf::setThreadAlias(const std::string name)
    {
        std::cout << "New thread alias: " << name << std::endl;
    }
    void TestCxxProf::shutdown()
    {
        std::cout << "Shutdown called" << std::endl;
    }

    std::string TestCxxProf::toString() const
    {
        return "TestCxxProf";
    }

}
