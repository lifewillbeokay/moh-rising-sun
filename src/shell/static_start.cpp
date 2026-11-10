// CStaticScreen::StartLoading chooses the loading screen's image for a level and mission, by the profile's
// language where it applies, and loads the progress bar and medal sprites. Part of the original StaticScreen.cpp.
#include "CameraShake.h"
#include "GameSettings.h"
extern "C" int sprintf(char *, const char *, ...);
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
class CStaticScreen {
  public:
    virtual ~CStaticScreen();
    void StartLoading(int, int, ELanguages, bool);
    int unknown00;
    CSprite *sprite04; // +0x04
    CSprite *sprite08; // +0x08
    int unknown0c;
    char name[64];     // +0x10
};

void CStaticScreen::StartLoading(int level, int mission, ELanguages language, bool flag) {
    if (g_GameSettings.skuType == 1) {
        switch (*(int *)g_GameSettings.profile) {
        case 2:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\spa\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\spa\\load%d_%d.gsh", level, level, mission);
            break;
        case 3:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\it\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\it\\load%d_%d.gsh", level, level, mission);
            break;
        case 6:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\nl\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\nl\\load%d_%d.gsh", level, level, mission);
            break;
        case 7:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\sw\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\sw\\load%d_%d.gsh", level, level, mission);
            break;
        case 8:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\dm\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\dm\\load%d_%d.gsh", level, level, mission);
            break;
        default:
            if (level == 1 && mission == 16 && !flag)
                sprintf(name, "data\\loading\\return.gsh");
            else
                sprintf(name, "data\\loading\\%d\\load%d_%d.gsh", level, level, mission);
            break;
        }
    } else {
        if (level == 1 && mission == 16)
            sprintf(name, "data\\loading\\return.gsh");
        else
            sprintf(name, "data\\loading\\%d\\load%d_%d.gsh", level, level, mission);
    }
    sprite04 = new ("source/shell/StaticScreen.cpp:213") CSprite("data\\loading\\loadbar.gsh", (CSprite::ESpriteLoadType)2);
    sprite08 = new ("source/shell/StaticScreen.cpp:214") CSprite("data\\loading\\medal.gsh", (CSprite::ESpriteLoadType)2);
    unknown0c = 0;
}
