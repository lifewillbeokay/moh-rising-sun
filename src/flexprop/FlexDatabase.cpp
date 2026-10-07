// AI-assisted reconstruction using the attributed STLport subset.
// See docs/FlexProp.md; the private vector and iterator remain original storage.
#include "profile.h"
#include <vector>
#include "FlexProp.h"
#include "FlexPropDatabase.h"

extern _STL::vector<FlexPropFormat *> g_properties;
extern _STL::vector<FlexPropFormat *>::iterator g_properties_iterator;

void FlexPropDatabase::Rewind() {
    g_properties_iterator = g_properties.begin();
}

int FlexPropDatabase::GetNumProperties() {
    return g_properties.size();
}

void FlexPropDatabase::GetPropertyByIndex(int index, FlexProp &out) {
    out.format = g_properties[index];
}
