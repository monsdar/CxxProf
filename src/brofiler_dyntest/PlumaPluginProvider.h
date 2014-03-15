
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "brofiler/IDynBrofiler.h"
#include "brofiler/IDynBrofilerProvider.h"

#include "brofiler_dyntest/TestBrofiler.h"

PLUMA_INHERIT_PROVIDER(TestBrofiler, IDynBrofiler);

#endif //_PLUMA_PLUGIN_PROVIDER_H_
