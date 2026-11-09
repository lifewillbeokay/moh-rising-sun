// Adapted from CC0 moh-frontline bsmachin_props.cpp; AI-assisted GR8E69 reconstruction.
// See docs/research/broad-source-sweep.md for target differences and provenance.
#include "PropertyReaders.h"

int _LoadPropertyData(MOH_core_Struct *object, int field) {
    float converted[3];
    switch (field) {
    case 0:
        return object->field36;
    case 2:
        return object->field38;
    case 4:
        return object->field35;
    case 5:
        return object->field48;
    case 7:
        return object->field50;
    case 8:
        return object->field2c;
    case 9:
        converted[0] = object->field2e / 16.0f;
        return *reinterpret_cast<int *>(&converted[0]);
    case 10:
        converted[1] = object->field30 / 16.0f;
        return *reinterpret_cast<int *>(&converted[1]);
    case 11:
        converted[2] = object->field32 / 16.0f;
        return *reinterpret_cast<int *>(&converted[2]);
    case 18:
        return object->flag18;
    case 19:
        return object->field3a;
    case 20:
        return object->flag20;
    case 21:
        return object->field3c;
    }
    return 0;
}
