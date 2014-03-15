
#include "brofiler/Brofiler.h"
#include "brofiler/IDynBrofilerProvider.h"

#include <iostream>

Brofiler::Brofiler(boost::shared_ptr<IDynBrofiler> givenDynBrofider) :
    dynBrofiler_(givenDynBrofider)
{}

Brofiler::~Brofiler()
{}

void Brofiler::loadDynBrofiler()
{
    pluma::Pluma manager;
    manager.acceptProviderType< IDynBrofilerProvider >();
    manager.loadFromFolder(".");
    
    std::vector<IDynBrofilerProvider*> providers;
    manager.getProviders(providers);
    
    std::cout << "Loaded " << providers.size() << " DynBrofilers" << std::endl;
}

boost::shared_ptr<IActivity> Brofiler::createActivity(const std::string& name) const
{
    return dynBrofiler_->createActivity(name);
}
