
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
        //TODO: We probably should add a mutex here? o0
        if (instance_ == NULL)
        {
            std::cout << "Creating CxxProfStatic" << std::endl;
            instance_ = new CxxProfStatic();
        }

        return instance_;
    }

    CxxProfStatic::CxxProfStatic() :
        manager_(new pluma::Pluma())
    {
        //load the plugin during instantiation of the CxxProfStatic
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

        //load all found plugins into our vector
        //all loaded plugins will be used together, so it is possible to have a ProfPlugin
        //storing the profiling data in a database while another sends it via
        //network to another application
        std::vector<IDynCxxProfProvider*>::iterator provIter = providers.begin();
        for (; provIter != providers.end(); ++provIter)
        {
            dynCxxProfs_.push_back((*provIter)->create());
            std::vector<IDynCxxProf*>::reverse_iterator pluginIter = dynCxxProfs_.rbegin();
            std::cout << "Loaded DynCxxProf: " << (*pluginIter)->toString() << std::endl;
        }

        if (dynCxxProfs_.empty())
        {
            std::cout << "Cannot find Plugin, no DynCxxProf loaded..." << std::endl;
        }
    }

    void CxxProfStatic::initialize()
    {
        //Nothing to do here for now
        //This part later could be used to initialize via configuration or something similar
    }

    std::vector< boost::shared_ptr<IActivity> > CxxProfStatic::createActivities(const std::string& name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //this is the vector which contains all the activites our plugins are creating
        std::vector< boost::shared_ptr<IActivity> > returnVec;

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            returnVec.push_back((*profIter)->createActivity(name));
        }

        return returnVec;
    }

    void CxxProfStatic::addMark(const std::string& name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            (*profIter)->addMark(name);
        }
    }

    void CxxProfStatic::addPlotValue(const std::string& name, double value)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            (*profIter)->addPlotValue(name, value);
        }
    }

    void CxxProfStatic::setProcessAlias(const std::string name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            (*profIter)->setProcessAlias(name);
        }
    }
    void CxxProfStatic::setThreadAlias(const std::string name)
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            (*profIter)->setThreadAlias(name);
        }
    }

    void CxxProfStatic::shutdown()
    {
        //this mutex protects the dynCxxProf_
        boost::mutex::scoped_lock lock(mutex_);

        //if no profilers are loaded, the following won't do anything...
        std::vector<IDynCxxProf*>::iterator profIter = dynCxxProfs_.begin();
        for (; profIter != dynCxxProfs_.end(); ++profIter)
        {
            (*profIter)->shutdown();
        }
    }

} //namespace CxxProf
