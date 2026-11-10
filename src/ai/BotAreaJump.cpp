// AI-assisted reconstruction from GR8E69. A fragment of the original source/BOT/BotArea.cpp;
// function and variable names come from the symbol table, member names are inferred.
#include "BotArea.h"

static JumpData s_JumpData[32];

JumpData *CBotArea::GetJumpData() {
    if (jumpIndex < 0)
        return 0;
    return &s_JumpData[jumpIndex];
}
