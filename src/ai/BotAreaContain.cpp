// AI-assisted reconstruction from GR8E69. A fragment of the original source/BOT/BotArea.cpp;
// function and variable names come from the symbol table, member names are inferred.
#include "BotArea.h"

int FindContainingArea(CVector3 &point) {
    int network = CAIAreaPathFinding::WhichNetwork(point);
    for (int i = 0; i < g_iNumPathFindingAreas; i++) {
        CAreaPathNode *node = AIPathFinding::GetPathNodeByIndex(i);
        if (node->network == network && node->ContainsPoint(point))
            return i;
    }
    return -1;
}
