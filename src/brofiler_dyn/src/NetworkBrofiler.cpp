
#include "brofiler_dyn/NetworkBrofiler.h"
#include "brofiler_dyn/NetworkActivity.h"

#include <boost/bind.hpp>
#include <iostream>

NetworkBrofiler::NetworkBrofiler() :
    actCounter_(1)
{
	//zero means that there are no activities yet
	activeActivity_.push(0);
}

NetworkBrofiler::~NetworkBrofiler()
{}

boost::shared_ptr<IActivity> NetworkBrofiler::createActivity(const std::string& name)
{
	unsigned int newActId = actCounter_++;

    boost::shared_ptr<NetworkActivity> newAct( new NetworkActivity() );
    newAct->setName(name);
	newAct->setActId(newActId);
	newAct->setParentId(activeActivity_.top());
    newAct->setThreadId(0);
    newAct->setResultCallback( boost::bind( &NetworkBrofiler::addResult, this, _1 ) );
    
    newAct->start();
	activeActivity_.push(newActId);
    
    return newAct;
}

void NetworkBrofiler::addResult(const ActivityResult& result)
{
    std::cout << "Activity " << result.Name << " ended. Parent: " << result.ParentId << "..." << std::endl;
	activeActivity_.pop();
	std::cout << "Current active Activity: " << activeActivity_.top() << std::endl;
	
}

std::string NetworkBrofiler::toString() const
{
    return "NetworkBrofiler";
}
