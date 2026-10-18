// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "Light.h"

void BIFunc_CreateLightByType(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float x = *(float *)(*stack - (count - 2));
    unsigned int type = *(*stack - (count - 1));
    float y = *(float *)(*stack - (count - 3));
    float z = *(float *)(*stack - (count - 4));
    int attach = *(*stack - (count - 5));
    CVector3 offset(x, y, z);
    CLight *light = CLight::Create(type);
    if (light) {
        if (object && attach)
            ((ISceneNode *)object)->SetAttachedLight(light, offset);
        else {
            CMatrix matrix;
            if (!CMatrix::s_ClassInit)
                CMatrix::InitClass();
            g_pBSObject->nativeObject->GetMatrix(matrix);
            matrix.PreTranslate(offset);
            light->SetTMLocalToWorld(matrix);
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)light;
}
