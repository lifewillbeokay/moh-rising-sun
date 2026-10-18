// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_DetachPlayerFromProp(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *prop = (BSObject *)args[1];
    if (!prop)
        prop = g_pBSObject;
    CAttachableObject *attachable = (CAttachableObject *)prop->gameObject->GetSceneNode();
    attachable->DetachObject((ISceneNode *)g_scene.GetPlayer(0));
    CHeadsUpDisplay *hud = &g_scene.GetPlayer(0)->hud;
    if (hud->field_7cc == 0.0f)
        hud->field_7cc = 1.0f;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
