// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
#include "CQuaternion.h"

// Descriptive inline helpers; no original constructor names are claimed.
static inline void SetLegacyRotation(CQuaternion &q, float x, float y, float z, float w) {
    q.w = w;
    q.x = x;
    q.y = y;
    q.z = z;
}

static inline CMatrix *InitLocalMatrix(CMatrix *matrix) {
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    return matrix;
}

void TriggerObject::GetForward(CVector3 &result) const {
    CMatrix matrix;
    CMatrix *m = InitLocalMatrix(&matrix);
    properties.flex.GetMatrix(*m);
    result = matrix.GetFront();
}

void TriggerObject::GetMatrix(CMatrix &result) const {
    if (flags.flexFormat)
        properties.flex.GetMatrix(result);
    else {
        xyzProperty_Struct *record = properties.legacy;
        CQuaternion rotation;
        SetLegacyRotation(rotation, record->field20, record->field24, record->field28, record->field1c);
        CVector3 position;
        position.Set(record->field10, record->field14, record->field18);
        rotation.GetMatrix(result);
        result.SetPos(position);
    }
}
