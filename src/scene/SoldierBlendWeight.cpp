// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart, so member names are descriptive.
#include "SceneObjects.h"

extern "C" void MUTEX_lock(void *);
extern "C" void MUTEX_unlock(void *);

void CSoldierObject::SetLipsynchBlendWeight(float weight) {
    if (weight < 0.0f)
        weight = 0.0f;
    else if (!(weight <= 1.0f))
        weight = 1.0f;
    void *mutex = lipsyncMutex;
    MUTEX_lock(mutex);
    lipsyncBlendWeight = weight;
    MUTEX_unlock(mutex);
}
