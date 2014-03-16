
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "brofiler/IDynBrofiler.h"
#include "brofiler/IDynBrofilerProvider.h"

#include "brofiler_dyn/NetworkBrofiler.h"

PLUMA_INHERIT_PROVIDER(NetworkBrofiler, IDynBrofiler);

#endif //_PLUMA_PLUGIN_PROVIDER_H_
