// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "SceneNode.h"

void MUSIC_Fade(float, int);
void MUSIC_SetLatency(int);

void BIFunc_PathfinderFadeVolume(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int volume = *(*stack - (count - 1));
    int time = *(*stack - (count - 2));
    MUSIC_Fade(volume * 0.01f, time);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PathfinderSetLatency(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    MUSIC_SetLatency(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

// Original 80-byte storage; this declaration does not allocate it.
extern unsigned char g_pGenericSoundMemory[80];

void BIFunc_GetGenericSoundStructureMemory(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)g_pGenericSoundMemory;
}

void BIFunc_FloatToInt(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float value = reinterpret_cast<float *>(args)[1];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = static_cast<int>(value);
}

void BIFunc_InitToInvalid(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}

void BIFunc_ResetParticleSystemRotation(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    IMovingSceneNode *system = (IMovingSceneNode *)args[1];
    if (system) {
        CMatrix matrix;
        CMatrix *m = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        system->GetTMLocalToWorld(*m);
        CVector3 position = m->GetPos();
        m->GetSlot(0);
        m->SetPos(position);
        system->SetTMLocalToWorld(*m);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
