
#include "cxxprof_static/CxxProfStatic.h"
#include "cxxprof_static/IDynCxxProfProvider.h"

#include <exception>
#include <iostream>

namespace CxxProf
{
    const std::string PLUGINDIR = ".";
    CxxProfStatic* CxxProfStatic::instance_ = NULL;

    CxxProfStatic* CxxProfStatic::getCxxProf()
    {
        if (instance_ == NULL)
        {
            std::cout << "Creating CxxProfStatic" << std::endl;
            instance_ = new CxxProfStatic();
        }

        return instance_;
    }

    CxxProfStatic::CxxProfStatic() :
        dynCxxProf_(NULL),
        manager_(new pluma::Pluma())
    {
        loadDynCxxProf();
    }

    CxxProfStatic::~CxxProfStatic()
    {
        instance_ = NULL;
    }

    void CxxProfStatic::loadDynCxxProf()
    {
        manager_->acceptProviderType< IDynCxxProfProvider >();
        manager_->loadFromFolder(PLUGINDIR);

        std::vector<IDynCxxProfProvider*> providers;
        manager_->getProviders(providers);

        if (providers.begin() != providers.end())
        {
            dynCxxProf_ = (*providers.begin())->create();
            std::cout << "Loaded DynCxxProf: " << dynCxxProf_->toString() << std::endl;
        }
        else
        {
            std::cout << "Cannot find Plugin, no DynCxxProf loaded..." << std::endl;
        }
    }

    void CxxProfStatic::initialize()
    {
        //Nothing to do here for now
        //This part later could be used to initialize via configuration or something similar
    }

    boost::shared_ptr<IActivity> CxxProfStatic::createActivity(const std::string& name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        if (dynCxxProf_ == NULL)
        {
            return boost::shared_ptr<IActivity>();
        }

        return dynCxxProf_->createActivity(name);
    }

    void CxxProfStatic::addMark(const std::string& name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        if (dynCxxProf_ != NULL)
        {
            dynCxxProf_->addMark(name);
        }
    }

    void CxxProfStatic::addPlotValue(const std::string& name, double value)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        if (dynCxxProf_ != NULL)
        {
            dynCxxProf_->addPlotValue(name, value);
        }
    }

    void CxxProfStatic::shutdown()
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        if (dynCxxProf_ != NULL)
        {
            dynCxxProf_->shutdown();
        }
    }

} //namespace CxxProf
