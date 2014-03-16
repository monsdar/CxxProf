
#include "brofiler_dyntest/TestActivity.h"

#include <iostream>

TestActivity::TestActivity(const std::string& name) :
    name_(name)
{
    std::cout << "Activity " << name_ << " started..." << std::endl;
}

TestActivity::~TestActivity()
{
    std::cout << "Activity " << name_ << " stopped..." << std::endl;
}