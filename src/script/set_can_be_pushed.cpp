// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetCanBePushed(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int canBePushed = *(*stack - (count - 2));
    if (!soldier)
        soldier = g_pBSObject;
    soldier->gameObject->GetSceneNode()->AsSoldierObject()->canBePushed = canBePushed;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
