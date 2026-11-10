// AI-assisted reconstruction of the sound manager's bank name checks; see docs/Sound.md.
#include "SoundManager.h"
extern "C" int sprintf(char *, const char *, ...);

// The names are built and never used: whatever checked them is gone from this build.
void cSoundMan::VerifyLevelBanks() {
    for (int i = 1; i <= 32; i++) {
        if (bankTable.banks[i - 1]) {
            char path[128];
            char name[128];
            sprintf(path, "data\\%d\\%d_%d\\", levelNumber, levelNumber, levelVariant);
            if (i == 31)
                sprintf(name, "%s%d%dscr.abk", path, levelNumber, levelVariant);
            else if (i == 32)
                sprintf(name, "%s%d%dstr.ast", path, levelNumber, levelVariant);
            else
                sprintf(name, "%s%d%dl%d.abk", path, levelNumber, levelVariant, i);
        }
    }
}
