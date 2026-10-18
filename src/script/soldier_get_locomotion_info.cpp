// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_Soldier_GetLocomotionInfo(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)args[1];
    int *info = &soldier->gameObject->GetSceneNode()->AsSoldierObject()->locomotionInfo;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)info;
}

void BIFunc_Soldier_EnableSkeletalPartition(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int which = *(*stack - (count - 2));
    CSoldierObject *object = soldier->gameObject->GetSceneNode()->AsSoldierObject();
    CSoldierObject::Partition partition = CSoldierObject::PartitionNone;
    switch (which) {
    case 1:
        partition = CSoldierObject::Partition2;
        break;
    case 2:
        partition = CSoldierObject::Partition3;
        break;
    }
    object->EnableSkeletalPartition(partition);
}
