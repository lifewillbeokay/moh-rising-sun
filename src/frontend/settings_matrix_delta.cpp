// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "GameSettings.h"
#include "CMatrix.h"
#ifdef DECOMP_COMPARE
void CGameSettings::SerializeDelta(const CMatrix &base, CMatrix &value) {
    const float *defaults = reinterpret_cast<const float *>(&base);
    float *values = reinterpret_cast<float *>(&value);
    bool same = true;
    if (SerializingOut()) {
        for (unsigned int i = 0; i < 16; ++i) {
            if (defaults[i] != values[i]) { same = false; break; }
        }
    }
    SerializeValue(same);
    if (!same) {
        for (unsigned int i = 0; i < 16; ++i) {
            bool changed = false;
            if (SerializingOut() && defaults[i] != values[i]) changed = true;
            SerializeValue(changed);
            if (changed) SerializeValue(values[i]);
        }
    }
}
#endif
