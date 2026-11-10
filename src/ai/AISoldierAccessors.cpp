// AI-assisted reconstruction from GR8E69. Fragments of the original AISoldierObject.cpp;
// method names come from the symbol table.
#include "AIBehavior.h"

void CAISoldierObject::OverrideLOSPosition(const CVector3 &position) {
    losOverride = 1;
    losOverridePosition = position;
}

void CAISoldierObject::StopOverrideLOSPosition() { losOverride = 0; }
