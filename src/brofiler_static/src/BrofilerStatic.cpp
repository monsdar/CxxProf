
#include "brofiler_static/BrofilerStatic.h"
#include "brofiler_static/IDynBrofilerProvider.h"

#include <exception>
#include <iostream>

const std::string PLUGINDIR = ".";

BrofilerStatic* BrofilerStatic::instance_ = NULL;

BrofilerStatic* BrofilerStatic::getBrofiler()
{
    if (instance_ == NULL)
    {
        std::cout << "Creating BrofilerStatic" << std::endl;
        instance_ = new BrofilerStatic();
    }

    return instance_;
}

BrofilerStatic::BrofilerStatic() :
    dynBrofiler_(NULL),
    manager_( new pluma::Pluma() )
{
    loadDynBrofiler();
}

BrofilerStatic::~BrofilerStatic()
{
    instance_ = NULL;
}

void BrofilerStatic::loadDynBrofiler()
{
    manager_->acceptProviderType< IDynBrofilerProvider >();
    manager_->loadFromFolder(PLUGINDIR);

    std::vector<IDynBrofilerProvider*> providers;
    manager_->getProviders(providers);

    if (providers.begin() != providers.end())
    {
        dynBrofiler_ = (*providers.begin())->create();
        std::cout << "Loaded DynBrofiler: " << dynBrofiler_->toString() << std::endl;
    }
    else
    {
        std::cout << "Cannot find Plugin, no DynBrofiler loaded..." << std::endl;
    }
}

boost::shared_ptr<IActivity> BrofilerStatic::createActivity(const std::string& name)
{
    //this mutex protects the dynBrofiler_
    boost::mutex::scoped_lock lock(mutex_);

    if(dynBrofiler_ == NULL)
    {
        return boost::shared_ptr<IActivity>();
    }
    
    return dynBrofiler_->createActivity(name);
}

void BrofilerStatic::addMark(const std::string& name)
{
    //this mutex protects the dynBrofiler_
    boost::mutex::scoped_lock lock(mutex_);

    if (dynBrofiler_ != NULL)
    {
        dynBrofiler_->addMark(name);
    }
}

void BrofilerStatic::addPlotValue(const std::string& name, double value)
{
    //this mutex protects the dynBrofiler_
    boost::mutex::scoped_lock lock(mutex_);

    if (dynBrofiler_ != NULL)
    {
        dynBrofiler_->addPlotValue(name, value);
    }
}

void BrofilerStatic::shutdown()
{
    //this mutex protects the dynBrofiler_
    boost::mutex::scoped_lock lock(mutex_);

    if (dynBrofiler_ != NULL)
    {
        dynBrofiler_->shutdown();
    }
}
