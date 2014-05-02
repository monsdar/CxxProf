
#ifndef _ACTIVITY_RESULT_H_
#define _ACTIVITY_RESULT_H_

#include "cxxprof_dyn_network/common.h"
#include <boost/cstdint.hpp>
#include <stdint.h>
#include <string>

namespace CxxProf
{

    struct CxxProf_Dyn_Network_EXPORT ActivityResult
    {
        std::string Name;
        unsigned int ThreadId;
        unsigned int ActId;
        unsigned int ParentId;
        boost::int64_t Starttime;
        boost::int64_t Stoptime;

        ActivityResult() :
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
