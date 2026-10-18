// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "Bullet.h"
#include "AIBehavior.h"

void BIFunc_CatchGrenade(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    CThrownBullet *grenade = (CThrownBullet *)*(*stack - (count - 2));
    ((CGrenadeResponseBehavior *)soldier->gameObject->GetSceneNode()->GetAIObject()->FindMatchingBehavior(BehaviorTypeGrenadeResponse))->caught = 1;
    grenade->SetCollisionId((EClsnId)10);
    grenade->SomebodyCaughtYou();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
