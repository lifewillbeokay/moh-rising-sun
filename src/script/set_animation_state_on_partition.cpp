// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAnimationStateOnPartition(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    CSoldierObject *object = soldier->gameObject->GetSceneNode()->AsSoldierObject();
    const char *state = (const char *)*(*stack - (count - 2));
    int flags = *(*stack - (count - 3));
    int which = *(*stack - (count - 4));
    CSoldierObject::Partition partition;
    if (which == 1)
        partition = CSoldierObject::Partition2;
    else if (which == 2)
        partition = CSoldierObject::Partition3;
    else
        partition = CSoldierObject::Partition1;
    object->SetAnimationState(state, flags, partition);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
