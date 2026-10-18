// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "CameraShake.h"

void BIFunc_AttachHierBaseToMMG(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = args[1];
    if (index >= 0) {
        CStaticObject *base = g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index);
        g_scene.GetPlayer(0)->mmgBase = base;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
