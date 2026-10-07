// Private database storage remains owned by the original flexprop.cpp unit.
// Uses the existing attributed STLport subset; see docs/FlexProp.md.
#ifndef FLEXPROP_DATABASE_INTERNAL_H
#define FLEXPROP_DATABASE_INTERNAL_H
#include "profile.h"
#include <map>
#include <string>
#include <vector>
#include "FlexProp.h"
#include "FlexPropDatabase.h"
extern _STL::map<_STL::string, FlexPropClassFormat *> g_classes;
extern _STL::map<_STL::string, FlexPropFormat *> g_properties_map;
extern _STL::vector<FlexPropFormat *> g_properties;
extern _STL::vector<FlexPropFormat *>::iterator g_properties_iterator;
#endif
