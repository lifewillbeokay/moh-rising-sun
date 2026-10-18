// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AISplinePath.h"

void BIFunc_IsOnAStarMesh(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *object = (BSObject *)args[1];
    CVector3 position;
    object->gameObject->GetSceneNode()->GetPosition(position);
    int result = AIPathFinding::IsPointOnNetwork(position);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
