// Adapted from CC0 moh-frontline bsmachin_props.cpp; AI-assisted GR8E69 reconstruction.
// See docs/research/broad-source-sweep.md for target differences and provenance.
#include "PropertyReaders.h"

int _LoadPropertyData(MOH_mechanic_Struct *object, int field) {
    float converted[3];
    switch (field) {
    case 0:
        return object->core.field36;
    case 2:
        return object->core.field38;
    case 4:
        return object->core.field35;
    case 5:
        return object->core.field48;
    case 7:
        return object->core.field50;
    case 8:
        return object->core.field2c;
    case 9:
        converted[0] = object->core.field2e / 16.0f;
        return *reinterpret_cast<int *>(&converted[0]);
    case 10:
        converted[1] = object->core.field30 / 16.0f;
        return *reinterpret_cast<int *>(&converted[1]);
    case 11:
        converted[2] = object->core.field32 / 16.0f;
        return *reinterpret_cast<int *>(&converted[2]);
    case 18:
        return object->core.flag18;
    case 19:
        return object->core.field3a;
    case 20:
        return object->core.flag20;
    case 21:
        return object->core.field3c;
    case 22:
        return object->field70;
    case 23:
        return object->field74;
    case 24:
        return object->field71;
    }
    return 0;
}
