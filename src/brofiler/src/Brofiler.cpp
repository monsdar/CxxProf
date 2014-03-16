
#include "brofiler/Brofiler.h"
#include "brofiler/IDynBrofilerProvider.h"

#include <exception>
#include <iostream>

const std::string PLUGINDIR = ".";

Brofiler::Brofiler() :
    dynBrofiler_(NULL),
    manager_( new pluma::Pluma() )
{}

Brofiler::~Brofiler()
{}

void Brofiler::loadDynBrofiler()
{
    manager_->acceptProviderType< IDynBrofilerProvider >();
    manager_->loadFromFolder(PLUGINDIR);
    
    std::vector<IDynBrofilerProvider*> providers;
    manager_->getProviders(providers);
    
    std::cout << "Loaded " << providers.size() << " DynBrofilerProviders" << std::endl;
    if (providers.begin() != providers.end())
    {
        std::cout << "Loaded a DynBrofiler" << std::endl;
        dynBrofiler_ = (*providers.begin())->create();
    }
    else
    {
        std::cout << "No DynBrofiler loaded..." << std::endl;
    }
}

boost::shared_ptr<IActivity> Brofiler::createActivity(const std::string& name) const
{
    if(dynBrofiler_ == NULL)
    {
        return boost::shared_ptr<IActivity>();
    }
    
    return dynBrofiler_->createActivity(name);
}
