// AI-assisted reconstruction from GR8E69. A fragment of the original BotMan source: function
// names come from the symbol table, member names are inferred.
#include "AIBot.h"

CVector3 *CampingPoint::GetLookat(unsigned char index) {
    return &g_BotMan.lookats[lookatIndices[index]].position;
}
