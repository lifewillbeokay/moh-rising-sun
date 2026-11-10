// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEScript.h"
#include "FEColor.h"

bool FEKeyTrack::SetKeyValueAt(long time, FEVector3 &value) {
    bool created = false;
    FEVector3 &base = *(FEVector3 *)&baseKey.value;
    if (time == -1) {
        base.x = value.x;
        base.y = value.y;
        base.z = value.z;
    } else {
        FEKeyNode *key = GetKeyAt(time);
        if (key->time != time) {
            key = CreateKeyAt(time);
            created = true;
        }
        FEVector3 delta = value - base;
        FEVector3 &out = *(FEVector3 *)&key->value;
        out.x = delta.x;
        out.y = delta.y;
        out.z = delta.z;
    }
    flags &= 0x7f;
    return created;
}

bool FEKeyTrack::SetKeyValueAt(long time, FEColor &value) {
    bool created = false;
    FEColor &base = *(FEColor *)&baseKey.value;
    if (time == -1) {
        base = value;
    } else {
        FEKeyNode *key = GetKeyAt(time);
        if (key->time != time) {
            key = CreateKeyAt(time);
            created = true;
        }
        *(FEColor *)&key->value = value - base;
    }
    flags &= 0x7f;
    return created;
}

#endif
