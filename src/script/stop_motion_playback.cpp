// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_StopMotionPlayback(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int index = args[1];
    if (index < 0)
        g_pBSObject->gameObject->GetSceneNode()->AsStaticObject()->StopMotionPlayback();
    else
        g_pBSObject->gameObject->GetSceneNode()->AsHierObject()->GetSubObject(index)->StopMotionPlayback();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
