// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart, so member names are descriptive.
#include "SceneObjects.h"

extern "C" int strcasecmp(const char *, const char *);

// An active slot with this name, or else the first free slot, which takes the name.
int CSoldierObject::GetAttachPoint(const char *name) {
    for (int i = 0; i < 8; i++) {
        if (attachSlots[i].name && strcasecmp(attachSlots[i].name, name) == 0 && attachSlots[i].active)
            return i;
    }
    for (int j = 0; j < 8; j++) {
        if (!attachSlots[j].active) {
            attachSlots[j].name = name;
            return j;
        }
    }
    return -1;
}
