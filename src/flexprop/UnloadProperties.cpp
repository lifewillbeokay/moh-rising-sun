// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "database_internal.h"

void FlexPropDatabase::UnloadProperties() {
    g_properties.clear();
    g_properties.~vector();
    new (&g_properties) _STL::vector<FlexPropFormat *>;
    g_properties_map.clear();
    g_properties_iterator = g_properties.end();
}
