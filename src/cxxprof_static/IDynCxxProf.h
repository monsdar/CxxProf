
#ifndef _I_DYN_CXXPROF_H_
#define _I_DYN_CXXPROF_H_

#include "cxxprof_static/IActivity.h"
#include <boost/shared_ptr.hpp>

namespace CxxProf
{

    class IDynCxxProf
    {
    public:
        /**
         * The virtual dtor is needed in order for the plugin to get destroyed cleanly
         */
        virtual ~IDynCxxProf() {};
        
        /**
         * This creates a new Activity which is active until the focus is lost in which it is created.
         *
         * Activities are measuring the time from their creation until they're destroyed. They're the heart
         * of CxxProf and should be put around all calls you want to measure.
         *
         * In short: Activities measure how long certain things take
         */
        virtual boost::shared_ptr<IActivity> createActivity(const std::string& name) = 0;
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
        virtual void addMark(const std::string& name) = 0;
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
        virtual void addPlotValue(const std::string& name, double value) = 0;

        /**
         * Sets the alias for the current process. CxxProf will use the given alias for this process
         * in all the data that is being published.
         * If no alias is set, CxxProf will use a automatically generated ID.
         */
        virtual void setProcessAlias(const std::string name) = 0;
        /**
         * Sets the alias for the current thread. CxxProf will automatically set the alias
         * to the thread where this command is called from
         * If no alias is set, CxxProf will use a automatically generated ID.
         */
        virtual void setThreadAlias(const std::string name) = 0;

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
        virtual void shutdown() = 0;

        /**
         * This should return the name of the DynCxxProf
         * It is called during instantiation of the plugin to show what has been loaded
         */
        virtual std::string toString() const = 0;
    };

} //namespace CxxProf

#endif //_I_DYN_CXXPROF_H_
