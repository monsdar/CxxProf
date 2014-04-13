
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "brofiler_static/IDynBrofiler.h"
#include "brofiler_static/IDynBrofilerProvider.h"

#include "brofiler_dyn_test/TestBrofiler.h"

PLUMA_INHERIT_PROVIDER(TestBrofiler, IDynBrofiler);

#endif //_PLUMA_PLUGIN_PROVIDER_H_
