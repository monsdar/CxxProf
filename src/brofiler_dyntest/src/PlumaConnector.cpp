

#include <Pluma/Connector.hpp>

#include "brofiler_dyntest/PlumaPluginProvider.h"

PLUMA_CONNECTOR
bool connect(pluma::Host& host){
    host.add( new TestBrofilerProvider() );
    return true;
}
