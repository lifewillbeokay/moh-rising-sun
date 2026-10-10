// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
long long MathFunGetRandomPercent();
int MathFunTestPercent(long long, long long);
void CAITargeting::ChooseSniperAimMode(AIFILTER_WEAPON_AIM_MODE *mode) {
    if (*mode == WeaponAimModeUnset) {
        if (MathFunTestPercent(MathFunGetRandomPercent(), 10))
            *mode = WeaponAimMode1;
        else
            *mode = WeaponAimMode2;
    }
}
