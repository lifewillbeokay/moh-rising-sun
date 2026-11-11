// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngMath.h"
bool Close(float, float, float);
void FEObject::SetPosition(const FEVector3 &value, bool relative) {
    if (type > 255) return;
    FEVector3 &position = ((FEObjData *)data)->position;
    bool unchanged;
    if (relative) unchanged = Close(value.x, 0.0f, 0.000001f) &&
        Close(value.y, 0.0f, 0.000001f) && Close(value.z, 0.0f, 0.000001f);
    else unchanged = Close(value.x, position.x, 0.000001f) &&
        Close(value.y, position.y, 0.000001f) && Close(value.z, position.z, 0.000001f);
    if (!unchanged) flags |= 0x400000;
    SetTrackValue((FEKeyTrack_Indices)2, value, relative);
}
#endif
