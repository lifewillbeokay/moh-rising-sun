// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "MPWeaponSet.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_CreateMPDumpObject(int **stack, void *object) {
    TriggerObject *trigger;
    if (object)
        trigger = ((BSObject *)((ISceneNode *)object)->GetScriptObject())->nativeObject;
    else
        trigger = g_pBSObject->nativeObject;
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    bool flag = *(bool *)(*stack - (count - 2));
    int team = *(*stack - (count - 3));
    EPowerupType type = (EPowerupType)*(*stack - (count - 4));
    unsigned int crc = AssetCRC(name);
    int result = 0;
    if (crc && crc != -1) {
        CMatrix matrix;
        CMatrix *m = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        trigger->GetRotationMatrix(*m);
        CVector3 position;
        if (object)
            ((ISceneNode *)object)->GetPosition(position);
        else
            trigger->GetPosition(position);
        matrix.SetPos(position);
        result = CreateDumpObject(matrix, crc, flag, g_pBSObject, team, type);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_CreateDumpObject(int **stack, void *object) {
    TriggerObject *trigger;
    if (object)
        trigger = ((BSObject *)((ISceneNode *)object)->GetScriptObject())->nativeObject;
    else
        trigger = g_pBSObject->nativeObject;
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    bool flag = *(bool *)(*stack - (count - 2));
    unsigned int crc = AssetCRC(name);
    int result = 0;
    if (crc && crc != -1) {
        CMatrix matrix;
        CMatrix *m = &matrix;
        if (!CMatrix::s_ClassInit)
            CMatrix::InitClass();
        trigger->GetRotationMatrix(*m);
        CVector3 position;
        if (object)
            ((ISceneNode *)object)->GetPosition(position);
        else
            trigger->GetPosition(position);
        matrix.SetPos(position);
        if (!g_pBSObject->gameObject.operator->())
            result = CreateDumpObject(matrix, crc, flag, g_pBSObject, 0, PowerupTypeUnknown0);
        else
            result = CreateDumpObject(matrix, crc, flag, 0, 0, PowerupTypeUnknown0);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
