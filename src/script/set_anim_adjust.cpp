// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SetAnimAdjust(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *animated = (BSObject *)*(*stack - (count - 1));
    BSObject *location = (BSObject *)*(*stack - (count - 2));
    bool flag = *(bool *)(*stack - (count - 3));
    CMatrix matrix;
    CMatrix *m = &matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    location->nativeObject->GetRotationMatrix(*m);
    float x, y, z;
    m->ToEulerXYZ(x, y, z);
    CVector3 position;
    location->nativeObject->GetPosition(position);
    animated->gameObject->GetSceneNode()->AsAnimObject()->StartAnimAdjust(position, z, flag);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
