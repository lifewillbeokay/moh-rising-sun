// AI-assisted reconstruction of the sound system front end's restore and heap selection; see docs/Sound.md.
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
int System::Restore() {
    SNDSYS_restore();
    pheap = 0;
    return 0;
}

int Memory::SetHeap(Device device, void *heap, int size) {
    switch (device) {
    case Device_Main:
        pheap = heap;
        heapsize = size;
        break;
    case Device_Fast: {
        SNDSYSRECORDHEAD opts;
        SNDSYSRECORDHEAD *record = &opts;
        SNDSYS_getopts(record);
        opts.fastHeap = heap;
        opts.fastHeapSize = size;
        SNDSYS_setopts((SNDSYSRECORD *)record);
        break;
    }
    default:
        return -5;
    }
    return 0;
}

int Memory::SetHeapFastRam(void *, int) {
    return -5;
}
}
