// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_GetMountedMachineGunByLinkCode(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *gun = (ISceneNode *)CMountedMachineGunBehavior::GetGunByLinkCode(args[1]);
    int result = 0;
    if (gun)
        result = gun->GetScriptObject();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
