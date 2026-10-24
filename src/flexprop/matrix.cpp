// AI-assisted reconstruction from GR8E69; see docs/FlexProp.md.
#include "FlexProp.h"
void FlexProp::GetMatrix(CMatrix &result) const {
    FlexPropFormat *data = format;
    CVector3 right, front, up, position;
    CVector3 *r = &right, *u = &up, *p = &position, *f = &front;
    right.x = data->transform[0];
    right.y = data->transform[1];
    r->z = data->transform[2];
    front.x = data->transform[3];
    front.y = data->transform[4];
    front.z = data->transform[5];
    up.x = data->transform[6];
    up.y = data->transform[7];
    up.z = data->transform[8];
    position.x = data->transform[9];
    position.y = data->transform[10];
    position.z = data->transform[11];
    result.GetSlot(0);
    result.SetRight(*r);
    result.SetFront(*f);
    result.SetUp(*u);
    result.SetPos(*p);
}
