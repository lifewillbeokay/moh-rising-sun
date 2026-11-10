// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

void CBotObject::SetMoveState(eBotMoveState state) {
    if (state == 2)
        crouching = 0;
    if (moveState != state)
        moveState = state;
}
