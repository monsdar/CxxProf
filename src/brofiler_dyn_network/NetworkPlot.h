
#ifndef _NETWORK_PLOT_H_
#define _NETWORK_PLOT_H_

#include "brofiler_dyn_network/common.h"
#include <string>

struct Brofiler_Dyn_Network_EXPORT NetworkPlot
{
    std::string Name;
    unsigned int Timestamp;
    double Value;
    
    NetworkPlot::NetworkPlot() :
        Name(""),
        Timestamp(0),
        Value(0.0)
    {}
};

#endif //_NETWORK_PLOT_H_
