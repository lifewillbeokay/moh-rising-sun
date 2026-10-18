// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AITargeting.h"

void BIFunc_RegisterAStarLinkPoint(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CAIAreaPathFinding::RegisterAStarLinkPoint(g_pBSObject->nativeObject, *(*stack - (count - 1)),
                                               *(*stack - (count - 2)), *(*stack - (count - 3)));
}

void BIFunc_RegisterAStarVolume(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CAIAreaPathFinding::RegisterAStarVolume(g_pBSObject->nativeObject, args[1]);
}
