
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "brofiler_static/IDynBrofiler.h"
#include "brofiler_static/IDynBrofilerProvider.h"

#include "brofiler_dyn_network/NetworkBrofiler.h"

namespace Brofiler
{

    PLUMA_INHERIT_PROVIDER(NetworkBrofiler, IDynBrofiler);

}

#endif //_PLUMA_PLUGIN_PROVIDER_H_
