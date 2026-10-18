// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetGravity(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int gravity = args[1];
    ((ISceneNode *)object)->AsAnimObject()->gravity = gravity;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetGroundCheck(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int groundCheck = args[1];
    ((ISceneNode *)object)->AsAnimObject()->groundCheck = groundCheck;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_InduceGroundCheck(int **stack, void *object) {
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    if (soldier->groundCheckPending) {
        soldier->groundCheckState = 0;
        BSObjectTriggerEvent(soldier->scriptObject, 141, 0, 0, true);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
