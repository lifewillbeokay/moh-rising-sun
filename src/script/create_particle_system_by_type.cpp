// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ParticleClock.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"
#include "Weapon.h"
#include "ParticleRecipe.h"

void BIFunc_CreateParticleSystemByType(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int type = *(*stack - (count - 1));
    CVector3 offset(*(float *)(*stack - (count - 2)), *(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)));
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    if (object)
        ((ISceneNode *)object)->GetTMLocalToWorld(matrix);
    else
        g_pBSObject->nativeObject->GetMatrix(matrix);
    matrix.PreTranslate(offset);
    IParticleSystem *system = CPSManager::Create(type, true, eParticleSystemDefault);
    if (system) {
        system->SetTMLocalToWorld(matrix);
        if (!system->IsEternal())
            system = 0;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)system;
}

void BIFunc_CreateParticleSystemByName(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    CVector3 offset(*(float *)(*stack - (count - 2)), *(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)));
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    if (object)
        ((ISceneNode *)object)->GetTMLocalToWorld(matrix);
    else
        g_pBSObject->nativeObject->GetMatrix(matrix);
    matrix.PreTranslate(offset);
    IParticleSystem *system = CPSManager::Create(AssetCRC(name), true, eParticleSystemDefault);
    if (system) {
        system->SetTMLocalToWorld(matrix);
        if (!system->IsEternal())
            system = 0;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)system;
}

void BIFunc_CreateParticleSystemByNameAtAttachPoint(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    unsigned int flags = *(*stack - (count - 2));
    BSObject *target = (BSObject *)*(*stack - (count - 3));
    ISceneNode *node = target->gameObject->GetSceneNode();
    IParticleSystem *system = CPSManager::Create(AssetCRC(name), true, eParticleSystemDefault);
    if (node && system) {
        system->Attach(*node, 0, flags);
        if (!system->IsEternal())
            system = 0;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)system;
}

void BIFunc_CreateParticleSystemByNameAtWeaponsAttachPoint(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    unsigned int flags = *(*stack - (count - 2));
    BSObject *target = (BSObject *)*(*stack - (count - 3));
    CSoldierObject *soldier = target->gameObject->GetSceneNode()->AsSoldierObject();
    ISceneNode *weapon = (ISceneNode *)soldier->weapons[soldier->currentWeaponSlot];
    IParticleSystem *system = CPSManager::Create(AssetCRC(name), true, eParticleSystemDefault);
    if (weapon && system) {
        system->Attach(*weapon, 0, flags);
        if (!system->IsEternal())
            system = 0;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)system;
}
