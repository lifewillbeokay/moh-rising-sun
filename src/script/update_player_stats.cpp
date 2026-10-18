// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "CameraShake.h"
#include "Weapon.h"
#include "GameSettings.h"

extern int g_bInMultiplayerMode;

void BIFunc_UpdatePlayerStats(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int stat = *(*stack - (count - 1));
    unsigned int value = *(*stack - (count - 2));
    switch (stat) {
    case 0:
        if (!g_bInMultiplayerMode) {
            CPlayerObject *player = g_pBSObject->gameObject->GetSceneNode()->AsPlayerObject();
            if (player) {
                CWeapon *weapon = player->GetCurrentWeapon();
                if (weapon && weapon->crc == 0x4f5286c5)
                    break;
            }
            g_GameSettings.Set_shotsFired(value, false);
        }
        break;
    case 1:
        g_GameSettings.Set_hits(value, false);
        break;
    case 2:
        g_GameSettings.Set_hitsTaken(value, false);
        break;
    case 3:
        g_GameSettings.Set_enemiesKilled(value, false);
        break;
    case 4:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)0, false);
        break;
    case 5:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)1, false);
        break;
    case 6:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)2, false);
        break;
    case 7:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)3, false);
        break;
    case 8:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)4, false);
        break;
    case 9:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)5, false);
        break;
    case 10:
        g_GameSettings.Set_shotLocation(value, (EShotLocations)6, false);
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
