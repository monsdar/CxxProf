
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
};

#endif //_ACTIVITY_RESULT_H_
