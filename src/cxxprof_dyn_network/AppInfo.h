
#ifndef _APP_INFO_H_
#define _APP_INFO_H_

#include "cxxprof_dyn_network/common.h"
#include <map>
#include <string>

namespace CxxProf
{

    struct CxxProf_Dyn_Network_EXPORT AppInfo
    {
        std::string Name;
        unsigned int Starttime;
        std::map<unsigned int, std::string> ThreadAliases;

        AppInfo::AppInfo() :
            Name(""),
            Starttime(0)
        {}
    };

}

#endif //_APP_INFO_H_
