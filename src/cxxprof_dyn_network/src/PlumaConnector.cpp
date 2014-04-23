

#include <Pluma/Connector.hpp>

#include "cxxprof_dyn_network/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new CxxProf::NetworkCxxProfProvider() );
    return true;
}
