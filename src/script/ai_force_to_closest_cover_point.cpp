// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_AIForceToClosestCoverPoint(int **stack, void *object) {
    CCoverpointBehavior *cover = (CCoverpointBehavior *)((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->FindMatchingBehavior(BehaviorTypeCoverpoint);
    if (cover) {
        ((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->EnableBehavior(BehaviorTypeCoverpoint);
        cover->ForceToCoverPoint();
        cover->UpdateHeuristicScore();
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIForceToClosestMMG(int **stack, void *object) {
    CMountedMachineGunBehavior *mmg = (CMountedMachineGunBehavior *)((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->FindMatchingBehavior(BehaviorTypeMountedMachineGun);
    if (mmg) {
        ((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->EnableBehavior(BehaviorTypeMountedMachineGun);
        mmg->ForceToMMG();
        mmg->UpdateHeuristicScore();
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_AIForceToMMGFeeder(int **stack, void *object) {
    CMountedMachineGunBehavior *mmg = (CMountedMachineGunBehavior *)((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->FindMatchingBehavior(BehaviorTypeMountedMachineGun);
    if (mmg) {
        ((ISceneNode *)((ISceneNode *)object)->AsSoldierObject())->GetAIObject()->EnableBehavior(BehaviorTypeMountedMachineGun);
        mmg->ForceToMMGFeeder();
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
