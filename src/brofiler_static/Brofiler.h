
#ifndef _BROFILER_H_
#define _BROFILER_H_

#ifdef USEBROFILER

#include <boost/shared_ptr.hpp>

#include <brofiler_static/BrofilerStatic.h>
#include <brofiler_static/IActivity.h>

#define BROFILER_ACTIVITY(NAME) \
    boost::shared_ptr<IActivity> BROFILER_HELPER_COMBINE(newActivity, __LINE__) = BrofilerStatic::getBrofiler()->createActivity(NAME);

#define BROFILER_MARK(NAME) \
    BrofilerStatic::getBrofiler()->addMark(NAME);

#define BROFILER_PLOT(NAME, VALUE) \
    BrofilerStatic::getBrofiler()->addPlotValue(NAME, VALUE);

// helper macros, see stackoverflow question #10379691
#define BROFILER_HELPER_COMBINE1(X,Y) X##Y  
#define BROFILER_HELPER_COMBINE(X,Y) BROFILER_HELPER_COMBINE1(X,Y)

#else

//Define empty macros
#define BROFILER_ACTIVITY(NAME) 
#define BROFILER_MARK(NAME) 
#define BROFILER_PLOT(NAME, VALUE) 

#endif //USEBROFILER

#endif //_BROFILER_H_
