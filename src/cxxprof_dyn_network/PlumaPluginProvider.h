
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "cxxprof_static/IDynCxxProf.h"
#include "cxxprof_static/IDynCxxProfProvider.h"

#include "cxxprof_dyn_network/NetworkCxxProf.h"

namespace CxxProf
{

    PLUMA_INHERIT_PROVIDER(NetworkCxxProf, IDynCxxProf);

}

#endif //_PLUMA_PLUGIN_PROVIDER_H_
