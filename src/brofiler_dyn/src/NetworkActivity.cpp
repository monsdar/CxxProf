
#include "brofiler_dyn/NetworkActivity.h"

#include <iostream>

NetworkActivity::NetworkActivity(const std::string& name) :
    name_(name)
{
    std::cout << "Activity " << name_ << " started..." << std::endl;
}

NetworkActivity::~NetworkActivity()
{
    std::cout << "Activity " << name_ << " stopped..." << std::endl;
}
