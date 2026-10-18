// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_ResetMMG(int **stack, void *) {
    CMatrix matrix;
    CMatrix *m = &matrix;
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    IMovingSceneNode *gun = (IMovingSceneNode *)CMountedMachineGunBehavior::GetGunByLinkCode(args[1]);
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    ((BSObject *)gun->GetScriptObject())->nativeObject->GetMatrix(*m);
    gun->SetTMLocalToWorld(*m);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
