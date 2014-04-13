

#include <Pluma/Connector.hpp>

#include "brofiler_dyn_test/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new TestBrofilerProvider() );
    return true;
}
