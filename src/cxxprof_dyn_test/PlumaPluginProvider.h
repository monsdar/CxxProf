
#ifndef _PLUMA_PLUGIN_PROVIDER_H_
#define _PLUMA_PLUGIN_PROVIDER_H_

#include <Pluma/Pluma.hpp>

#include "cxxprof_static/IDynCxxProf.h"
#include "cxxprof_static/IDynCxxProfProvider.h"

#include "cxxprof_dyn_test/TestCxxProf.h"

namespace CxxProf
{

    PLUMA_INHERIT_PROVIDER(TestCxxProf, IDynCxxProf);

}

#endif //_PLUMA_PLUGIN_PROVIDER_H_
