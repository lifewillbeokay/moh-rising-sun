// A fragment of the original toilet.cpp: the animation bank, the optional MVD file, sound banks and the
// level contents. Names come from the symbol table; inferred names are marked in Toilet.h and below.
#include "Toilet.h"
#include <stdio.h>
#include <string.h>

extern "C" int DVDConvertPathToEntrynum(const char *);
void TLT_LoadFile_Async_Start(const char *, int);
bool TLT_LoadFile_Async_Load(char **, int *);
void TLT_CloseFile(void *);
bool IsSoundDisabled();
void SoundLoadBank(unsigned char *);
void LoadOneShotSoundMem(int, char *);

struct SSaveData {
    unsigned char mission; // +0x000
    unsigned char level;   // +0x001
};
struct SCoreGameSettings {
    unsigned char unknown00[0x31664];
    SSaveData saveData; // +0x31664
};
class CGameSettings {
  public:
    int GetCurrentOneShotBankID();
    int GetCurrentSavePointID() const;
    SCoreGameSettings *profile; // +0x000
};
extern CGameSettings g_GameSettings;

void InitLevelSoundModules(int, int);
void LoadLevelSoundModules(int, int);
extern int g_bAudioIsDone;
void DebugMsg(const char *, ...);
extern char *szTLT_LevelContentsFileName;

class cSoundMan {
  public:
    void SetLevelPath(const char *path) { strcpy(levelPath, path); }
    unsigned char unknown00[0x862c];
    char levelPath[0x86ac - 0x862c]; // +0x862c
};
extern cSoundMan g_SoundMan;

extern char *VMStartLoc;
extern int MVDExists;
extern char *szTLT_MVDBigFileName;

static int TLT_Load_WaitAnimation() {
    char *buffer;
    int size;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        g_toilet.animationBank = VMStartLoc;
        memcpy(VMStartLoc, buffer, size);
        operator delete(buffer);
        VMStartLoc += size + 32 - size % 32;
        g_toilet.animationBankSize = size;
        return 1;
    }
    return 0;
}

void *TLT_GetAnimationBank() { return g_toilet.animationBank; }

int TLT_GetAnimationBankSize() { return g_toilet.animationBankSize; }

static int TLT_Load_StartMVD() {
    char path[128];
    sprintf(path, "data/%d/%d_%d/%s", g_toilet.mission, g_toilet.mission, g_toilet.stage, szTLT_MVDBigFileName);
    if (DVDConvertPathToEntrynum(path) == -1)
        MVDExists = 0;
    else {
        MVDExists = 1;
        TLT_LoadFile_Async_Start(path, 0);
    }
    return 1;
}

static int TLT_Load_WaitMVD() {
    char *buffer;
    int size;
    if (!MVDExists)
        return 1;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        g_toilet.mvdBigFile = VMStartLoc;
        memcpy(VMStartLoc, buffer, size);
        operator delete(buffer);
        g_toilet.mvdBigFileSize = size;
        return 1;
    }
    return 0;
}

char *TLT_Sound_GetCurrentPath() {
    static char name[256] = "";
    switch (g_toilet.soundBankIndex) {
    case 0:
    case 1:
        if (g_toilet.mission == 2 || g_toilet.mission == 4)
            strcpy(name, "data\\2\\2_1\\sound\\");
        else
            sprintf(name, "data\\%d\\%d_%d\\sound\\", g_toilet.mission, g_toilet.mission, g_toilet.stage);
        break;
    }
    return name;
}

char *TLT_Sound_GetCurrentAbk() {
    static char asyncName[64] = "";
    char *result = 0;
    switch (g_toilet.soundBankIndex) {
    case 0:
        if (g_toilet.mission == 2 || g_toilet.mission == 4)
            strcpy(asyncName, "21Scr.abk");
        else
            sprintf(asyncName, "%d%dScr.abk", g_toilet.mission, g_toilet.stage);
        result = asyncName;
        break;
    case 1:
        if (g_GameSettings.GetCurrentSavePointID()) {
            if (g_GameSettings.GetCurrentOneShotBankID() >= 0)
                sprintf(asyncName, "%d%dl%d.abk", g_toilet.mission, g_toilet.stage, g_GameSettings.GetCurrentOneShotBankID());
        } else
            sprintf(asyncName, "%d%dl1.abk", g_toilet.mission, g_toilet.stage);
        result = asyncName;
        break;
    }
    return result;
}

static int TLT_Load_StartSound() {
    if (IsSoundDisabled())
        return 1;
    g_toilet.soundBankIndex = 0;
    return 1;
}

static int TLT_Load_StartSoundBank() {
    char path[256];
    if (IsSoundDisabled())
        return 1;
    char *directory = TLT_Sound_GetCurrentPath();
    char *bank = TLT_Sound_GetCurrentAbk();
    sprintf(path, "%s%s", directory, bank);
    TLT_LoadFile_Async_Start(path, 128);
    return 1;
}

static int TLT_Load_WaitSoundBank() {
    char *buffer;
    int size;
    if (IsSoundDisabled())
        return 1;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        if (TLT_Sound_GetCurrentAbk()) {
            if (g_toilet.soundBankIndex == 0)
                SoundLoadBank((unsigned char *)buffer);
            if (g_toilet.soundBankIndex == 1) {
                if (g_GameSettings.GetCurrentSavePointID()) {
                    if (g_GameSettings.GetCurrentOneShotBankID() >= 0)
                        LoadOneShotSoundMem(g_GameSettings.GetCurrentOneShotBankID(), buffer);
                } else
                    LoadOneShotSoundMem(1, buffer);
            }
            TLT_CloseFile(buffer);
        }
        g_toilet.soundBankIndex++;
        if (g_toilet.soundBankIndex > 0) {
            int mission = g_toilet.mission;
            if (mission >= 2 && mission <= 4)
                return 1;
        }
        if (g_toilet.soundBankIndex > 1)
            return 1;
        return -1;
    }
    return 0;
}

static int TLT_Load_SoundModules() {
    char path[128];
    InitLevelSoundModules(g_toilet.mission, g_toilet.stage);
    LoadLevelSoundModules(g_GameSettings.profile->saveData.mission, g_GameSettings.profile->saveData.level);
    sprintf(path, "data\\%d\\%d_%d\\", g_toilet.mission, g_toilet.mission, g_toilet.stage);
    g_SoundMan.SetLevelPath(path);
    g_bAudioIsDone = 1;
    return 1;
}

struct LevelContentsHeader {
    int version;                        // +0x00
    int checksum;                       // +0x04
    int entryCount;                     // +0x08
    LevelFileContentsStruct_ *entries;  // +0x0c; a file offset until rebased here.
};

static int TLT_Load_InitContent() {
    char name[256];
    int size;
    strcpy(name, szTLT_LevelContentsFileName);
    LevelContentsHeader *header = (LevelContentsHeader *)TLT_LoadFileFromLevelBigFile(name, &size);
    if (header->version != 2) {
        DebugMsg("Error: version check failed on level contents file\n");
        DebugMsg("       Engine is version %d.  File is version %d.\n", 2, header->version);
    }
    if (header->checksum != 0xa5658) {
        DebugMsg("Error: checksum failed on level contents file\n");
        DebugMsg("       Engine checksum = %d.  File checksum = %d.\n", 0xa5658, header->checksum);
        DebugMsg("Use Toilet to generate a new tltheader.h for the engine or get a new definition.crd and reprocess your level\n");
    }
    header->entries = (LevelFileContentsStruct_ *)((char *)header + (int)header->entries);
    g_toilet.contents = header->entries;
    g_toilet.contentCount = header->entryCount;
    for (int i = 0; i < header->entryCount; i++) {
        LevelFileContentsStruct_ *entry = &g_toilet.contents[i];
        entry->data = (TLTResourceEntry **)((char *)header + (int)entry->data);
        for (int j = 0; j < entry->count; j++) {
            entry->data[j] = (TLTResourceEntry *)((char *)header + (int)entry->data[j]);
            int type = entry->data[j]->unknown00;
            if (type == 0 || type == 4) {
                for (int k = 0; k < entry->data[j]->count; k++) {
                    int offset = entry->data[j]->values[k];
                    if (offset)
                        entry->data[j]->values[k] = (int)header + offset;
                }
            }
        }
    }
    g_toilet.contentsEnd = g_toilet.contents + header->entryCount;
    return 1;
}
