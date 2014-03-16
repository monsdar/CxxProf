
#include "brofiler_dyn/NetworkBrofiler.h"
#include "brofiler_dyn/NetworkActivity.h"

#include <boost/bind.hpp>
#include <iostream>

NetworkBrofiler::NetworkBrofiler() :
    actCounter_(1)
{}

NetworkBrofiler::~NetworkBrofiler()
{}

boost::shared_ptr<IActivity> NetworkBrofiler::createActivity(const std::string& name)
{
    boost::shared_ptr<NetworkActivity> newAct( new NetworkActivity() );
    newAct->setName(name);
    newAct->setActId(actCounter_++);
    newAct->setParentId(0);
    newAct->setThreadId(0);
    newAct->setResultCallback( boost::bind( &NetworkBrofiler::addResult, this, _1 ) );
    
    newAct->start();
    
    return newAct;
}

void NetworkBrofiler::addResult(const ActivityResult& result)
{
    std::cout << "Activity " << result.Name << " ended..." << std::endl;
}

std::string NetworkBrofiler::toString() const
{
    return "NetworkBrofiler";
}
