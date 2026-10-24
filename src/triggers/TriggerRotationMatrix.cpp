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

void TriggerObject::GetRotationMatrix(CMatrix &result) const {
    if (flags.flexFormat) {
        properties.flex.GetMatrix(result);
        CVector3 position;
        position.Set(0, 0, 0);
        result.SetPos(position);
    } else {
        xyzProperty_Struct *record = properties.legacy;
        CQuaternion rotation;
        SetLegacyRotation(rotation, record->field20, record->field24, record->field28, record->field1c);
        rotation.GetMatrix(result);
    }
}
