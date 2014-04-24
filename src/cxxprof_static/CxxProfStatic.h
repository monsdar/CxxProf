
#ifndef _CXXPROF_STATIC_H_
#define _CXXPROF_STATIC_H_

#include "cxxprof_static/common.h"
#include "cxxprof_static/IDynCxxProf.h"
#include "cxxprof_static/IActivity.h"

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

//forward declaration
namespace pluma
{
    class Pluma;
};

namespace CxxProf
{

    /**
     * This is a Singleton class which acts as the interface
     * to the DynCxxProf plugin.
     * During its creation it tries to load a plugin. If there is no plugin found
     * or the plugin is not loadable because of other reasons, all call will
     * return immediately.
     */
    class CxxProfStatic_EXPORT CxxProfStatic : boost::noncopyable
    {
    public:
        static CxxProfStatic* getCxxProf();
        virtual ~CxxProfStatic();
    
        /**
         * This creates a new Activity which is active until the focus is lost in which it is created.
         * If there is no plugin loaded, this will return an empty shared_ptr which does nothing at all.
         *
         * Activities are measuring the time from their creation until they're destroyed. They're the heart
         * of CxxProf and should be put around all calls you want to measure.
         *
         * In short: Activities measure how long certain things take
         */
        boost::shared_ptr<IActivity> createActivity(const std::string& name);
        /**
         * This creates a new Mark with the given name
         * 
         * Marks indicate that the state of the application has changed. An example would be
         * a Mark before the Initialization of an application and then before the usual execution.
         * This would indicate in which state the app is and what the analyst could expect. Certain
         * activities will only appear in certain states of the application, others will take
         * a totally different amount of time.
         *
         * In short: Marks define in which state the application is.
         */
        void addMark(const std::string& name);
        /**
         * This creates a Plot with the given name and value
         * Value will be casted into double internally.
         *
         * Plots are used to show how a certain name changes its value over time. They're needed to analyze
         * the context of the data. An example could be an application which handles a number of AI units. The developer
         * could add Plots every 200 milliseconds that show the number of active units being handled. Certain
         * activities will take longer when there are more units.
         *
         * In short: Plots show the context in which the data has to be read
         */
        void addPlotValue(const std::string& name, double value);
        /**
         * Sets the alias for the current process. CxxProf will use the given alias for this process
         * in all the data that is being published.
         * If no alias is set, CxxProf will use a automatically generated ID.
         */
        void setProcessAlias(const std::string name);
        /**
         * Sets the alias for the current thread. CxxProf will automatically set the alias
         * to the thread where this command is called from
         * If no alias is set, CxxProf will use a automatically generated ID.
         */
        void setThreadAlias(const std::string name);

        /**
         * This initializes the CxxProf environment
         * Call this method as early as possible, before any code you want to measure is executed
         * Usually this code is called in the beginning of the main
         *
         * Note: You do not need to call this method. If it is not called, CxxProf will initialize as soon as
         * you're using it for the first time. However this is probably a timepoint where you do not want it to do all the loading.
         */
        void initialize();
        /**
         * This shuts down Brofiler cleanly.
         * Cleanly means that it ends all active Activities and sends all the remaining data to the server.
         *
         * You should call this method at the very end of your application. Mostly it will be called at the
         * end of the main function.
         *
         * If you do not call this method, CxxProf will shutdown immediately and there is a good chance that
         * some of the last Activities you wanted to record will not get to the server.
         *
         * Note: This is a BLOCKING call, depending on the mass of data you're generating with CxxProf this call
         * could take some time
         */
        void shutdown();

    private:
        CxxProfStatic();
        static CxxProfStatic* instance_;

        void loadDynCxxProf();
        IDynCxxProf* dynCxxProf_;

        boost::mutex mutex_;
        boost::shared_ptr<pluma::Pluma> manager_;
    };

} //namespace CxxProf

#endif //_CXXPROF_STATIC_H_
