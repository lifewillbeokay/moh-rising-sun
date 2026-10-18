// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CMatrix.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_GlareCreateByType(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int type = *(*stack - (count - 1));
    CVector3 offset(*(float *)(*stack - (count - 2)), *(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)));
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    if (object) {
        ISceneNode *node = (ISceneNode *)object;
        node->GetTMLocalToWorld(matrix);
        if (node->AsStaticObject() && !((CStaticObjectInterface *)node->AsStaticObject())->AsThrownObject() &&
            !((CStaticObjectInterface *)node->AsStaticObject())->AsHierTankObject()) {
            BoundingVolumeView *volume = (BoundingVolumeView *)node->GetWorldBoundingVolume((ISceneNode::EVolumeType)3);
            if (volume) {
                CVector3 center = volume->center;
                matrix.SetPos(center);
            }
        }
    } else {
        g_pBSObject->nativeObject->GetMatrix(matrix);
    }
    matrix.PreTranslate(offset);
    IMovingSceneNode *glare = CGlare::Create(type, true);
    if (glare)
        glare->SetPosition(matrix.GetPos());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)glare;
}

void BIFunc_GlareCreateByName(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    char *name = (char *)*(*stack - (count - 1));
    CVector3 offset(*(float *)(*stack - (count - 2)), *(float *)(*stack - (count - 3)), *(float *)(*stack - (count - 4)));
    CMatrix matrix;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    if (object) {
        ISceneNode *node = (ISceneNode *)object;
        node->GetTMLocalToWorld(matrix);
        if (node->AsStaticObject() && !((CStaticObjectInterface *)node->AsStaticObject())->AsThrownObject() &&
            !((CStaticObjectInterface *)node->AsStaticObject())->AsHierTankObject()) {
            BoundingVolumeView *volume = (BoundingVolumeView *)node->GetWorldBoundingVolume((ISceneNode::EVolumeType)3);
            if (volume) {
                CVector3 center = volume->center;
                matrix.SetPos(center);
            }
        }
    } else {
        g_pBSObject->nativeObject->GetMatrix(matrix);
    }
    matrix.PreTranslate(offset);
    IMovingSceneNode *glare = CGlare::Create(AssetCRC(name), true);
    if (glare)
        glare->SetPosition(matrix.GetPos());
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = (int)glare;
}
