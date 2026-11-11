// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEScript.h"
void FEObject::SetTrackValue(FEKeyTrack_Indices index, const FEVector3 &value, bool relative) {
    for (FEScript *script = (FEScript *)scriptList.head; script; script = (FEScript *)script->next) {
        FEKeyTrack *track = script->FindTrack(index);
        if (track) {
            FEVector3 &base = *(FEVector3 *)&track->baseKey.value;
            if (relative) base += value;
            else { base.x = value.x; base.y = value.y; base.z = value.z; }
            track->flags &= 0x7f;
        }
    }
    FEVector3 &out = *(FEVector3 *)(data + GetDataOffset(index));
    if (relative) out += value;
    else { out.x = value.x; out.y = value.y; out.z = value.z; }
}
#endif
