
#ifndef _CXXPROF_H_
#define _CXXPROF_H_

#ifdef USECXXPROF

#include <boost/shared_ptr.hpp>

#include <cxxprof_static/CxxProfStatic.h>
#include <cxxprof_static/IActivity.h>

#define CXXPROF_INIT() \
    CxxProf::CxxProfStatic::getCxxProf()->initialize();

#define CXXPROF_ACTIVITY(NAME) \
    boost::shared_ptr<CxxProf::IActivity> CXXPROF_HELPER_COMBINE(newActivity, __LINE__) = CxxProf::CxxProfStatic::getCxxProf()->createActivity(NAME);

#define CXXPROF_MARK(NAME) \
    CxxProf::CxxProfStatic::getCxxProf()->addMark(NAME);

#define CXXPROF_PLOT(NAME, VALUE) \
    CxxProf::CxxProfStatic::getCxxProf()->addPlotValue(NAME, VALUE);

#define CXXPROF_SHUTDOWN() \
    CxxProf::CxxProfStatic::getCxxProf()->shutdown();

// helper macros, see stackoverflow question #10379691
// DO NOT USE THESE
#define CXXPROF_HELPER_COMBINE1(X,Y) X##Y  
#define CXXPROF_HELPER_COMBINE(X,Y) CXXPROF_HELPER_COMBINE1(X,Y)

#else

//Define empty macros
#define CXXPROF_INIT() 
#define CXXPROF_ACTIVITY(NAME) 
#define CXXPROF_MARK(NAME) 
#define CXXPROF_PLOT(NAME, VALUE) 
#define CXXPROF_SHUTDOWN() 

#define CXXPROF_HELPER_COMBINE1(X,Y) 
#define CXXPROF_HELPER_COMBINE(X,Y) 

#endif //USECXXPROF

#endif //_CXXPROF_H_
