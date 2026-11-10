// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"
#include "MathFun.h"

void CBotObject::BS_Enter_Camp() {
    SetMoveState((eBotMoveState)0);
    SetCampingLookat();
    int type = GetCurrentCampingPoint()->type;
    if (type == 0)
        crouching = 1;
    else if (type != 2 && MathFunRandomReal(0.0f, 1.0f) < character.skills[6])
        crouching = 1;
    else
        crouching = 0;
}

void CBotObject::BS_Exit_Camp() { crouching = 0; }
