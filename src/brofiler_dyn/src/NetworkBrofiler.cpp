
#include "brofiler_dyn/NetworkBrofiler.h"
#include "brofiler_dyn/NetworkActivity.h"

NetworkBrofiler::NetworkBrofiler()
{}

NetworkBrofiler::~NetworkBrofiler()
{}

boost::shared_ptr<IActivity> NetworkBrofiler::createActivity(const std::string& name) const
{
    return boost::shared_ptr<IActivity>( new NetworkActivity(name) );
}
