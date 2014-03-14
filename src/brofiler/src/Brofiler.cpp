
#include "brofiler/Brofiler.h"

Brofiler::Brofiler(boost::shared_ptr<IDynBrofiler> givenDynBrofider) :
    dynBrofiler_(givenDynBrofider)
{}

Brofiler::~Brofiler()
{}

boost::shared_ptr<IActivity> Brofiler::createActivity(const std::string& name) const
{
    return dynBrofiler_->createActivity(name);
}
