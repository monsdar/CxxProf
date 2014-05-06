
#ifndef _CXXPROF_H_
#define _CXXPROF_H_

/**
 * If USECXXPROF is defined, we're using CxxProf with all of its features.
 * This also means that, if the directive is not defined, we're just using empty macros which will be removed by the compiler
 * during compilation. Thus we're removing CxxProf completely from the code.
 */
#ifdef USECXXPROF

/**
 * We're using Shared_ptrs to delete Activities as soon as their focus is lost
 */
#include <boost/shared_ptr.hpp>

#include <cxxprof_static/CxxProfStatic.h>
#include <cxxprof_static/IActivity.h>

/**
 * This initializes the CxxProf environment
 * Call this macro as early as possible, before any code you want to measure is executed
 * Usually this code is called in the beginning of the main
 *
 * Note: You do not need to call this macro. If it is not called, CxxProf will initialize as soon as
 * you're using it for the first time. This is probably a timepoint where you do not want it to load.
 */
#define CXXPROF_INIT() \
    CxxProf::CxxProfStatic::getCxxProf()->initialize();

/**
 * This creates a new Activity which is active until the focus is lost in which it is created.
 *
 * Activities are measuring the time from their creation until they're destroyed. They're the heart
 * of CxxProf and should be put around all calls you want to measure.
 *
 * In short: Activities measure how long certain things take.
 *
 * You can create as many Activities as you want, the only limitation is that it isn't allowed
 * to create multiple Activities in the same line of code
 */
#define CXXPROF_ACTIVITY(NAME) \
    std::vector<boost::shared_ptr<CxxProf::IActivity> > CXXPROF_HELPER_COMBINE(newActivity, __LINE__) = CxxProf::CxxProfStatic::getCxxProf()->createActivities(NAME);

/**
 * This is the same as using CXXPROF_ACTIVITY, but an additional
 * condition can be given which needs to be matched.
 *
 * It is useful if you e.g. just want to start measuring if your application
 * reaches a certain state or if you have a certain number of objects created.
 */
#define CXXPROF_ACTIVITY_COND(NAME, COND) \
    if( COND )                 \
    {                          \
        CXXPROF_ACTIVITY(NAME) \
    }


/**
 * This creates a new Mark with the given NAME.
 * 
 * Marks indicate that the state of the application has changed. An example would be
 * a Mark before the Initialization of an application and then before the usual execution.
 * This would indicate in which state the app is and what the analyst could expect. Certain
 * activities will only appear in certain states of the application, others will take
 * a totally different amount of time.
 *
 * In short: Marks define in which state the application is.
 */
#define CXXPROF_MARK(NAME) \
    CxxProf::CxxProfStatic::getCxxProf()->addMark(NAME);

/**
* This is the same as using CXXPROF_MARK, but an additional
* condition can be given which needs to be matched.
*
* It is useful if you e.g. just want to start measuring if your application
* reaches a certain state or if you have a certain number of objects created.
*/
#define CXXPROF_MARK_COND(NAME, COND) \
    if( COND )             \
    {                      \
        CXXPROF_MARK(NAME) \
    }

/**
 * This creates a Plot with the given NAME and VALUE
 * VALUE will be casted into double internally.
 *
 * Plots are used to show how a certain NAME changes its VALUE over time. They're needed to analyze
 * the context of the data. An example could be an application which handles a number of AI units. The developer
 * could add Plots every 200 milliseconds that show the number of active units being handled. Certain
 * activities will take longer when there are more units.
 *
 * In short: Plots show the context in which the data has to be read
 */
#define CXXPROF_PLOT(NAME, VALUE) \
    CxxProf::CxxProfStatic::getCxxProf()->addPlotValue(NAME, VALUE);

/**
* This is the same as using CXXPROF_PLOT, but an additional
* condition can be given which needs to be matched.
*
* It is useful if you e.g. just want to start measuring if your application
* reaches a certain state or if you have a certain number of objects created.
*/
#define CXXPROF_PLOT_COND(NAME, VALUE, COND) \
    if( COND )                    \
    {                             \
        CXXPROF_PLOT(NAME, VALUE) \
    }

/**
 * This shuts down CxxProf cleanly.
 * Cleanly means that it ends all active Activities and sends all the remaining data to the server.
 *
 * You should call this macro at the very end of your application. Mostly it will be called at the
 * end of the main function.
 *
 * If you do not call this macro, CxxProf will shutdown immediately and there is a good chance that
 * some of the last Activities you wanted to record will not get to the server.
 *
 * Note: This is a BLOCKING call, depending on the mass of data you're generating with CxxProf this call
 * could take some time
 */
#define CXXPROF_SHUTDOWN() \
    CxxProf::CxxProfStatic::getCxxProf()->shutdown();

/**
 * Sets the alias for the current process. CxxProf will use the given alias for this process
 * in all the data that is being published.
 * If no alias is set, CxxProf will use a automatically generated ID.
 */
#define CXXPROF_PROCESS_ALIAS(NAME) \
    CxxProf::CxxProfStatic::getCxxProf()->setProcessAlias(NAME);

/**
 * Sets the alias for the current thread. CxxProf will automatically set the alias
 * to the thread where this command is called from
 * If no alias is set, CxxProf will use a automatically generated ID.
 */
#define CXXPROF_THREAD_ALIAS(NAME) \
    CxxProf::CxxProfStatic::getCxxProf()->setThreadAlias(NAME);

/**
 * DO NOT USE THESE
 * 
 * These are helper macros to help create unique object names for the activities.
 * They're using the current LINE to generate names like 'object123'
 *
 * See stackoverflow question #10379691 and #1597007 for more information
 */ 
#define CXXPROF_HELPER_COMBINE1(X,Y) X##Y  
#define CXXPROF_HELPER_COMBINE(X,Y) CXXPROF_HELPER_COMBINE1(X,Y)

#else

/**
 * When USECXXPROF is not defined we're just creating empty macros. This allows
 * us to let the compiler remove CxxProf completely from the sources during compilation time
 */
#define CXXPROF_INIT() 
#define CXXPROF_ACTIVITY(NAME)
#define CXXPROF_ACTIVITY_COND(NAME, COND) 
#define CXXPROF_MARK(NAME) 
#define CXXPROF_MARK_COND(NAME, COND) 
#define CXXPROF_PLOT(NAME, VALUE) 
#define CXXPROF_PLOT_COND(NAME, VALUE, COND) 
#define CXXPROF_SHUTDOWN() 
#define CXXPROF_PROCESS_ALIAS(NAME) 
#define CXXPROF_THREAD_ALIAS(NAME) 

/**
 * These shouldn't be needed to get removed, but you never know if the user used them for anything...
 * Hope they don't cause trouble then. Perhaps we should create an #error here...
 */
#define CXXPROF_HELPER_COMBINE1(X,Y) 
#define CXXPROF_HELPER_COMBINE(X,Y) 

#endif //USECXXPROF

#endif //_CXXPROF_H_
