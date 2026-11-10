// AI-assisted reconstruction from GR8E69. Fragments of the original AI.cpp: the level
// lifecycle of the AI subsystem. Names come from the symbol table.
#include "AIBehavior.h"
#include "ScriptedGag.h"

void ResetSearchNodePool();
void UnregisterAllNavPoints();

namespace AI {
void Initialize();
void Restart();
void Shutdown();
}

void AI::Initialize() {
    CGrenadeResponseBehavior::Initialize();
    ResetSearchNodePool();
}

void AI::Restart() {
    UnregisterAllNavPoints();
    CScriptedGag::UnregisterAllGagActions();
    CMountedMachineGunBehavior::DeregisterAllGuns();
    CMountedMachineGunBehavior::DeregisterAllFeeders();
    CMountedMachineGunBehavior::DeregisterMMGTriggers();
    CGrenadeResponseBehavior::Initialize();
    CCoverpointBehavior::ResetCoverpoints();
    CAITargeting::ResetLocationTargets();
    CAIAreaPathFinding::ResetAStarVolumes();
    AIPathFinding::ResetAreaPathNodes();
}

void AI::Shutdown() {
    UnregisterAllNavPoints();
    CScriptedGag::UnregisterAllGagActions();
    CMountedMachineGunBehavior::DeregisterAllGuns();
    CMountedMachineGunBehavior::DeregisterAllFeeders();
    CMountedMachineGunBehavior::DeregisterMMGTriggers();
    CGrenadeResponseBehavior::Shutdown();
    CCoverpointBehavior::ResetCoverpoints();
    CAITargeting::ResetLocationTargets();
    CAIAreaPathFinding::ResetAStarVolumes();
    AIPathFinding::ResetAreaPathNodes();
}
