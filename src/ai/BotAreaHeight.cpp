// AI-assisted reconstruction from GR8E69. A fragment of the original source/BOT/BotArea.cpp;
// function and variable names come from the symbol table, member names are inferred.
#include "BotArea.h"

float GetHeightDiff(CVector3 &a, CVector3 &b, CVector3 &c, CVector3 &d) {
    return (a.z + b.z) * 0.5f - (c.z + d.z) * 0.5f;
}
