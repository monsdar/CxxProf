
#ifndef _NETWORK_MARK_H_
#define _NETWORK_MARK_H_

#include "brofiler_dyn_network/common.h"
#include <string>

namespace Brofiler
{

    struct Brofiler_Dyn_Network_EXPORT NetworkMark
    {
        std::string Name;
        unsigned int Timestamp;

        NetworkMark::NetworkMark() :
            Name(""),
            Timestamp(0)
        {}
    };

}

#endif //_NETWORK_MARK_H_
