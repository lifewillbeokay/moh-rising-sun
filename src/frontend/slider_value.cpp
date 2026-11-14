// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "VolumeKnobs.h"
#include "FEScript.h"
#ifdef DECOMP_COMPARE
void CSlider::SetValue(float position) {
    FEKeyTrack *track = object->currentScript->FindTrack((FEKeyTrack_Indices)2);
    value = position;
    FEVector3 scaled;
    scaled.x = vectorB.x * position;
    scaled.z = vectorB.z * position;
    scaled.y = vectorB.y * position;
    FEVector3 translated;
    translated.x = vectorA.x + scaled.x;
    translated.z = vectorA.z + scaled.z;
    translated.y = vectorA.y + scaled.y;
    track->SetKeyValueAt(-1, translated);
    object->flags |= 0x400000;
}
#endif
