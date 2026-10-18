// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_PlayerEjectShell(int **stack, void *object) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int kind = args[1];
    CPlayerObject *player = ((ISceneNode *)object)->AsPlayerObject();
    CPlayerWeaponObject *weapon = player->GetCurrentPlayerWeapon();
    int shell;
    switch (kind) {
    case 0:
        shell = 0x1f3ea5cd;
        break;
    case 1:
        shell = 0x1f3ea5cd;
        break;
    case 3:
        shell = 0x57756861;
        break;
    case 4:
        shell = 0xa8626e1c;
        break;
    case 2:
    default:
        shell = 0x357a1c01;
        break;
    }
    if (weapon && !player->pathBits.noShells)
        weapon->CreateEjectedShell(shell);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
