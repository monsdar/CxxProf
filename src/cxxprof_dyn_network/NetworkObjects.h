
#ifndef _NETWORK_OBJECTS_H_
#define _NETWORK_OBJECTS_H_

#include "cxxprof_dyn_network/common.h"
#include "cxxprof_dyn_network/AppInfo.h"
#include "cxxprof_dyn_network/NetworkMark.h"
#include "cxxprof_dyn_network/NetworkPlot.h"
#include "cxxprof_dyn_network/ActivityResult.h"
#include <vector>

namespace CxxProf
{

    struct CxxProf_Dyn_Network_EXPORT NetworkObjects
    {
        AppInfo Info;
        std::vector<NetworkMark> Marks;
        std::vector<NetworkPlot> Plots;
        std::vector<ActivityResult> ActivityResults;

        void NetworkObjects::clear()
        {
            Marks.clear();
            Plots.clear();
            ActivityResults.clear();
        }

        unsigned int NetworkObjects::size() const
        {
            return Marks.size() + Plots.size() + ActivityResults.size();
        }
        unsigned int NetworkObjects::empty() const
        {
            return Marks.empty() || Plots.empty() || ActivityResults.empty();
        }
    };

}

#endif //_NETWORK_OBJECTS_H_
