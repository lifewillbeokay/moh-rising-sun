#include "ScriptBuiltins.h"
#include "SoundAmbience.h"
#include "ScriptRuntime.h"

void BIFunc_AmbientTrack_Azimuth(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    if (args[1]) {
        CVector3 position;
        g_pBSObject->nativeObject->GetPosition(position);
        AmbientTrack_SetLocation(&position);
    } else {
        AmbientTrack_SetLocation(0);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
