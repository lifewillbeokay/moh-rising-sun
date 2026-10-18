// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Bullet.h"

void BIFunc_Soldier_CreateExplosion(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    float radius = *(float *)(*stack - (count - 2));
    float damage = *(float *)(*stack - (count - 3));
    float x = *(float *)(*stack - (count - 4));
    float y = *(float *)(*stack - (count - 5));
    float z = *(float *)(*stack - (count - 6));
    float force = *(float *)(*stack - (count - 7));
    CSoldierObject *node = soldier->gameObject->GetSceneNode()->AsSoldierObject();
    CVector3 position;
    CVector3 offset(x, y, z);
    ((ISceneNode *)node)->GetPosition(position);
    position += offset;
    CExplosion explosion(g_scene, position, radius, damage, force, (EScriptBulletType)21, 0, true, true);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
