// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "SceneNode.h"

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
