// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "AISplinePath.h"

void BIFunc_PlayerStartPath(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int first = *(*stack - (count - 1));
    unsigned int second = *(*stack - (count - 2));
    float speed = *(float *)(*stack - (count - 3));
    CAISplinePath *firstPath;
    CAISplinePath *secondPath;
    if (first)
        firstPath = FindSplinePath(first);
    else
        firstPath = 0;
    if (second)
        secondPath = FindSplinePath(second);
    else
        secondPath = 0;
    g_scene.GetPlayer(0)->MoveOnPath(firstPath, secondPath, speed);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
