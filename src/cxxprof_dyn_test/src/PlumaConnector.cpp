

#include <Pluma/Connector.hpp>

#include "cxxprof_dyn_test/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new CxxProf::TestCxxProfProvider() );
    return true;
}
