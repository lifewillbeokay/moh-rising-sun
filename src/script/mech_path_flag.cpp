// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

// Descriptive storage annotation for the four-byte scale after the conversion double.
static const float pathScaleFactor[1]
    __attribute__((section(".rodata.path_scale"), aligned(4))) = { 0.01f };

void BIFunc_MechSetPathFlag(int **stack, void *object) {
    CStaticObject *mech = ((ISceneNode *)object)->AsStaticObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int flag = *(*stack - (count - 1));
    int value = *(*stack - (count - 2));
    switch (flag) {
    case 0:
    default:
        mech->pathScale = value * pathScaleFactor[0];
        break;
    case 1:
        mech->flags.pathFlag1 = value != 0;
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
