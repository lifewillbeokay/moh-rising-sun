// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "Bullet.h"

void BIFunc_CreateExplosion(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float radius = *(float *)(*stack - (count - 1));
    float damage = *(float *)(*stack - (count - 2));
    float x = *(float *)(*stack - (count - 3));
    float y = *(float *)(*stack - (count - 4));
    float z = *(float *)(*stack - (count - 5));
    float force = *(float *)(*stack - (count - 6));
    bool flag = *(bool *)(*stack - (count - 7));
    CVector3 position;
    CVector3 offset(x, y, z);
    if (object)
        ((ISceneNode *)object)->GetPosition(position);
    else
        g_pBSObject->nativeObject->GetPosition(position);
    position += offset;
    CExplosion explosion(g_scene, position, radius, damage, force, (EScriptBulletType)21, 0, true, flag);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
