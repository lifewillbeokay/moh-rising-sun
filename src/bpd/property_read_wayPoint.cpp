// Adapted from CC0 moh-frontline bsmachin_props.cpp; AI-assisted GR8E69 reconstruction.
// See docs/research/broad-source-sweep.md for target differences and provenance.
#include "PropertyReaders.h"

int _LoadPropertyData(MOH_wayPoint_Struct *object, int field) {
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
        return object->flag22;
    case 23:
        return object->flag23;
    case 24:
        return object->flag24;
    case 25:
        return object->flag25;
    case 26:
        return object->flag26;
    case 27:
        return object->flag27;
    case 28:
        return object->flag28;
    case 29:
        return object->field70 & 1;
    }
    return 0;
}
