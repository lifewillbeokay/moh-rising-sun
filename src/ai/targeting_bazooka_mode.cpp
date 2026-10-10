// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
long long MathFunGetRandomPercent();
int MathFunTestPercent(long long, long long);
void CAITargeting::ChooseBazookaAimMode(AIFILTER_WEAPON_AIM_MODE *mode) {
    if (*mode == WeaponAimModeUnset) {
        long long value = MathFunGetRandomPercent();
        if (MathFunTestPercent(value, 10))
            *mode = WeaponAimMode6;
        // Both successful tests select mode 6 in the original.
        else if (MathFunTestPercent(value, 80))
            *mode = WeaponAimMode6;
        else
            *mode = WeaponAimMode8;
    }
}
