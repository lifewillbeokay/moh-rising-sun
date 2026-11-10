// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table.
#include <stdlib.h>
#include "AIBot.h"
#include "MathFun.h"

void CBotObject::SetCampingLookat() {
    static float LOOK_TIME = 2.5f;
    CampingPoint *point = GetCurrentCampingPoint();
    int index = rand() % point->lookatCount;
    if (index == campingLookat && point->lookatCount > 1)
        index = (index + 1U) % point->lookatCount;
    campingLookat = index;
    CVector3 *lookat = point->GetLookat(index);
    SetLookTarget(*lookat);
    float scale = lookat->w;
    float time = scale * MathFunRandomReal(0.0f, LOOK_TIME) + 1.0f;
    nextLookTime = GetClock() + time;
}
