// Adapted from CC0 moh-frontline bsmachin_props.cpp; AI-assisted GR8E69 reconstruction.
// See docs/research/broad-source-sweep.md for target differences and provenance.
#include "PropertyReaders.h"

int _LoadPropertyData(MOH_enemy_Struct *object, int field) {
    float converted[11];
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
        return object->field90;
    case 24:
        return object->field71;
    case 25:
        return object->field94;
    case 26:
        return object->field72;
    case 27:
        return object->field98;
    case 28:
        return object->field9c;
    case 29:
        return object->fielda0;
    case 30:
        return object->field73;
    case 31:
        return object->field74;
    case 32:
        return object->field75;
    case 33:
        return object->fielda4;
    case 34:
        return object->fielda8;
    case 35:
        converted[3] = object->field7e / 16.0f;
        return *reinterpret_cast<int *>(&converted[3]);
    case 36:
        converted[4] = object->field80 / 16.0f;
        return *reinterpret_cast<int *>(&converted[4]);
    case 37:
        converted[5] = object->field82 / 16.0f;
        return *reinterpret_cast<int *>(&converted[5]);
    case 38:
        return object->field76;
    case 39:
        return object->field77;
    case 40:
        return object->field78;
    case 41:
        return object->field79;
    case 42:
        return object->field84;
    case 43:
        return object->field7a;
    case 44:
        return object->fieldac;
    case 45:
        converted[6] = object->field86 / 16.0f;
        return *reinterpret_cast<int *>(&converted[6]);
    case 46:
        converted[7] = object->field88 / 16.0f;
        return *reinterpret_cast<int *>(&converted[7]);
    case 47:
        converted[8] = object->field8a / 16.0f;
        return *reinterpret_cast<int *>(&converted[8]);
    case 48:
        converted[9] = object->field8c / 16.0f;
        return *reinterpret_cast<int *>(&converted[9]);
    case 49:
        converted[10] = object->field8e / 16.0f;
        return *reinterpret_cast<int *>(&converted[10]);
    case 50:
        return object->field7b;
    case 51:
        return object->field7c;
    }
    return 0;
}
