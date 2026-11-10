// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original attachobject.cpp; member names are descriptive.
#include "SceneObjects.h"

static inline void AlignMatrix(CMatrix &matrix, const CMatrix &inverse) {
    CMatrix::s_TempMat = matrix;
    matrix.Multiply(inverse, CMatrix::s_TempMat);
}

int CAttachableObject::AttachAndAlignObject(CAnimObject *object, int node) {
    int result = AttachObject(object, node);
    CMatrix world;
    CMatrix *worldp = &world;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    ((ISceneNode *)object)->GetTMLocalToWorld(*worldp);
    CMatrix inverse;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    inverse.FastInverse(*worldp);
    for (int i = 0; i < 20; i++) {
        if (children[i] == (ISceneNode *)object)
            AlignMatrix(childMatrices[i], inverse);
    }
    return result;
}
