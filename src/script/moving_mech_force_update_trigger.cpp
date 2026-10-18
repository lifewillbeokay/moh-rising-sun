// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void ForceUpdateProximityTriggerStatus(BSGO_Basic *, bool);

void BIFunc_MovingMechForceUpdateTrigger(int **stack, void *) {
    ForceUpdateProximityTriggerStatus(g_pBSObject->gameObject.operator->(), false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_MechSetFlag(int **stack, void *object) {
    CStaticObject *mech = ((ISceneNode *)object)->AsStaticObject();
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int bit = *(*stack - (count - 1));
    bool set = *(*stack - (count - 2)) != 0;
    if (set)
        mech->mechFlags |= 1 << bit;
    else
        mech->mechFlags &= ~(1 << bit);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
