
#ifndef _ACTIVITY_RESULT_H_
#define _ACTIVITY_RESULT_H_

#include "brofiler_dyn/common.h"

struct Brofiler_EXPORT ActivityResult
{
    std::string Name;
    unsigned int ThreadId;
    unsigned int ActId;
    unsigned int ParentId;
    unsigned int StartTime;
    unsigned int StopTime;
    
    ActivityResult::ActivityResult() :
        Name(""),
        ThreadId(0),
        ActId(0),
        ParentId(0),
        StartTime(0),
        StopTime(0)
    {}
};

#endif //_ACTIVITY_RESULT_H_
