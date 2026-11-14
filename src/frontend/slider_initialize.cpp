// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "VolumeKnobs.h"
#include "FEScript.h"
void CSlider::Initialize(FEObject *object) {
    FEScript *script = object->FindScript(FEHashUpper("MinMax"));
    FEKeyTrack *track = script->FindTrack((FEKeyTrack_Indices)2);
    vectorA = *(CSliderVector *)&track->baseKey.value;
    FEKeyNode *key = track->GetKeyAt(20);
    vectorB = *(CSliderVector *)&key->value;
    *(FEObject **)unknown00 = object;
}
#endif
