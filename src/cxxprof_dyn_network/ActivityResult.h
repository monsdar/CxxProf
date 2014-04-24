
#ifndef _ACTIVITY_RESULT_H_
#define _ACTIVITY_RESULT_H_

#include "cxxprof_dyn_network/common.h"
#include <string>

namespace CxxProf
{

    struct CxxProf_Dyn_Network_EXPORT ActivityResult
    {
        std::string Name;
        unsigned int ThreadId;
        unsigned int ActId;
        unsigned int ParentId;
        unsigned int Starttime;
        unsigned int Stoptime;

        ActivityResult::ActivityResult() :
            Name(""),
            ThreadId(0),
            ActId(0),
            ParentId(0),
            Starttime(0),
            Stoptime(0)
        {}
    };

}

#endif //_ACTIVITY_RESULT_H_
