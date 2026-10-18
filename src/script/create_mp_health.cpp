// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "MPWeaponSet.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"

void BIFunc_CreateMPHealth(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int item = g_pMPWeaponSet->data->health->items[args[1] - 1];
    if (item) {
        CMatrix matrix;
        CMatrix *m = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        g_pBSObject->nativeObject->GetMatrix(*m);
        CreateDumpObject(*m, item, true, g_pBSObject, 0, PowerupTypeHealth);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_CreateMPRankedAmmoWeapon(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int weapon = *(*stack - (count - 1));
    int team = *(*stack - (count - 2));
    int ranked = *(*stack - (count - 3));
    int item;
    if (weapon == 0) {
        if (ranked)
            item = g_pMPWeaponSet->data->rankedAmmo->items[0];
        else
            item = g_pMPWeaponSet->data->defaultAmmo->items[0];
    } else {
        item = g_pMPWeaponSet->data->weapons->items[weapon - 1];
        ranked = 0;
    }
    if (item) {
        CMatrix matrix;
        CMatrix *m = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        g_pBSObject->nativeObject->GetMatrix(*m);
        EPowerupType type = PowerupType3;
        if (ranked)
            type = PowerupType2;
        CreateDumpObject(*m, item, true, g_pBSObject, team, type);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
