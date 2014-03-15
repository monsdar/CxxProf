
#include "brofiler/Brofiler.h"
#include "brofiler/IDynBrofilerProvider.h"

#include <exception>
#include <iostream>

Brofiler::Brofiler() :
    dynBrofiler_(NULL),
    manager_( new pluma::Pluma() )
{}

Brofiler::~Brofiler()
{}

void Brofiler::loadDynBrofiler()
{
    manager_->acceptProviderType< IDynBrofilerProvider >();
    manager_->loadFromFolder(".");
    
    std::vector<IDynBrofilerProvider*> providers;
    manager_->getProviders(providers);
    
    if(providers.size() == 1)
    {
        std::cout << "Loaded a DynBrofiler" << std::endl;
        dynBrofiler_ = (*providers.begin())->create();
    }
    else if(providers.size() > 1)
    {
        std::cout << "Loaded " << providers.size() << " DynBrofilers, using the first one" << std::endl;
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
        throw std::exception();
    }
    
    return dynBrofiler_->createActivity(name);
}
