// AI-assisted reconstruction of the sound system front end's option setters and output mode; see docs/Sound.md.
#include "SndSystem.h"
struct SNDSYSRECORD;
struct SNDSYSRECORDHEAD {
    unsigned char unknown_00[5];
    unsigned char outputChannels; // +0x05; read by CapOutputMode.
    unsigned char unknown_06[0x28 - 0x06];
    unsigned short maxBanks; // +0x28
    unsigned char unknown_2a[0x2e - 0x2a];
    unsigned char mainVoices; // +0x2e
    unsigned char unknown_2f[0x33 - 0x2f];
    unsigned char fastVoices; // +0x33
    unsigned char unknown_34[0x47 - 0x34];
    unsigned char speakerMode; // +0x47
    unsigned char unknown_48[0x5c - 0x48];
    void *fastHeap; // +0x5c
    int fastHeapSize; // +0x60
    unsigned char unknown_64[212 - 0x64];
};
extern "C" int SNDSYS_getopts(SNDSYSRECORDHEAD *);
extern "C" int SNDSYS_setopts(SNDSYSRECORD *);
extern "C" int SNDSYS_restore();
extern "C" int SNDSYSI_init(void *, int, int);

namespace Snd {
int System::SetMaxBanks(int count) {
    SNDSYSRECORDHEAD opts;
    SNDSYSRECORDHEAD *record = &opts;
    SNDSYS_getopts(record);
    opts.maxBanks = count;
    SNDSYS_setopts((SNDSYSRECORD *)record);
    return 0;
}

int System::CapOutputMode(OutputMode mode, bool *supported) {
    SNDSYSRECORDHEAD opts;
    *supported = false;
    SNDSYS_getopts(&opts);
    switch (mode) {
    case OutputMode_1:
        if (opts.outputChannels > 1)
            *supported = true;
        break;
    case OutputMode_0:
    case OutputMode_8:
        *supported = true;
        break;
    case OutputMode_2:
    case OutputMode_3:
    case OutputMode_4:
    case OutputMode_5:
        break;
    }
    return 0;
}

int System::SetOutputMode(OutputMode mode) {
    bool supported;
    CapOutputMode(mode, &supported);
    int result;
    if (supported) {
        outputmode = mode;
        result = 0;
    } else {
        result = -5;
    }
    return result;
}
}
