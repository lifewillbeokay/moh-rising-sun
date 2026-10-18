// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_EnablePartitionOnOther(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int which = *(*stack - (count - 1));
    int enable = *(*stack - (count - 2));
    ISceneNode *other = (ISceneNode *)*(*stack - (count - 3));
    CSoldierObject *object = other->AsSoldierObject();
    CSoldierObject::Partition partition = CSoldierObject::PartitionNone;
    switch (which) {
    case 1:
        partition = CSoldierObject::Partition2;
        break;
    case 2:
        partition = CSoldierObject::Partition3;
        break;
    }
    if (enable)
        object->EnableSkeletalPartition(partition);
    else
        object->DisableSkeletalPartition(partition);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetAnimationStatePartitionOnOther(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    const char *state = (const char *)*(*stack - (count - 1));
    int flags = *(*stack - (count - 2));
    ISceneNode *other = (ISceneNode *)*(*stack - (count - 3));
    int which = *(*stack - (count - 4));
    CSoldierObject::Partition partition;
    if (which == 1)
        partition = CSoldierObject::Partition2;
    else if (which == 2)
        partition = CSoldierObject::Partition3;
    else
        partition = CSoldierObject::Partition1;
    other->AsSoldierObject()->SetAnimationState(state, flags, partition);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
