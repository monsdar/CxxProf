

#include <Pluma/Connector.hpp>

#include "brofiler_dyn_network/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new Brofiler::NetworkBrofilerProvider() );
    return true;
}
