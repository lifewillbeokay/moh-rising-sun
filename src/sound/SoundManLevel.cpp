// AI-assisted reconstruction of the sound manager's level loading; see docs/Sound.md.
#include "SoundManager.h"
bool TLT_FileExists(const char *);
extern "C" bool FILESYS_addbigsync(const char *, int, int, int *);
extern "C" bool FILESYS_delbigsync(const char *, int);
extern "C" void *FILE_loadbigheader(const char *, int);
extern "C" int DWI_free(void *);
extern "C" char *strcpy(char *, const char *);
extern "C" void *memset(void *, int, unsigned int);

cSoundMan::cSoundMan()
    : columns(0), hasBankTable(0), field_80b4(1.0f), soundVolume(1.0f) {
    levelStreamHandle = -1;
    soundZone = 0;
    streamCount = 0;
    streamBufferSize = 0;
    streamsReady = 0;
    levelNumber = 0;
    levelVariant = 0;
    field_86f8 = 0;
    field_86f4 = 0.0f;
    paused = 0;
    for (int i = 0; i < 4; i++) {
        streamBuffers[i] = 0;
        streams[i] = -12;
    }
    memset(&bankTable, 0, sizeof(bankTable));
}

void cSoundMan::InitLevel(unsigned char *data, unsigned char *buss, int level, int variant) {
    levelNumber = level;
    levelVariant = variant;
    soundZone = 0;
    paused = 0;
    soundData = data;
    LoadLevelUbr((UberFileHeader *)data);
    VerifyLevelBanks();
    if (buss) {
        bussFile = (UberFileHeader *)buss;
        LoadLevelBus((UberFileHeader *)buss);
    }
}

int cSoundMan::LoadLevelStreamFile(const char *name) {
    TLT_FileExists(name);
    FILESYS_addbigsync(name, 0, 50, &levelStreamHandle);
    levelStreamHeader = FILE_loadbigheader(name, 1024);
    strcpy(levelStreamName, name);
    return 0;
}

// The binary passes the handle word where the file system takes a name.
void cSoundMan::RemoveLevelStreamFile() {
    FILESYS_delbigsync((const char *)levelStreamHandle, 50);
    DWI_free(levelStreamHeader);
    levelStreamHandle = -1;
    levelStreamHeader = 0;
}
