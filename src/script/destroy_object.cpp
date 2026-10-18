// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "AITargeting.h"

void DelayDestroyObject(BSGO_Basic *);

void BIFunc_DestroyObject(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *node = (ISceneNode *)args[1];
    switch (g_pBSObject->nativeObject->GetNativeBindingType()) {
    case 6:
        node->MarkForDestruction(0);
        break;
    case 7: case 8: case 9:
        ((CAnimObjectInterface *)node)->CleanUp();
        break;
    default:
        BSObject *target = (BSObject *)node;
        if (!target)
            target = g_pBSObject;
        CAITargeting::DeRegisterLocationTarget((int)g_pBSObject);
        BSGO_Basic *gameObject = target->gameObject.operator->();
        if (gameObject)
            DelayDestroyObject(gameObject);
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
