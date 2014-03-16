

#include <Pluma/Connector.hpp>

#include "brofiler_dyn/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new NetworkBrofilerProvider() );
    return true;
}
