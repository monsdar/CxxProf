
#ifndef _NETWORK_PLOT_H_
#define _NETWORK_PLOT_H_

#include "cxxprof_dyn_network/common.h"
#include <string>

namespace CxxProf
{

    struct CxxProf_Dyn_Network_EXPORT NetworkPlot
    {
        std::string Name;
        unsigned int Timestamp;
        double Value;

        NetworkPlot() :
            Name(""),
            Timestamp(0),
            Value(0.0)
        {}
    };

}

#endif //_NETWORK_PLOT_H_
