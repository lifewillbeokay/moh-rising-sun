// A fragment of the original rcmpplayer.cpp: the movie player's heap setup and teardown, language
// column lookup, construction and the subtitle file names. Class and method names come from the symbol
// table; the member names and their use are inferred from this code.
#include "Rcmp.h"
#include "AvPlayer.h"
#include "VideoDriver.h"
#include <stdio.h>
#include <string.h>

extern "C" int DWI_free(void *);
extern "C" void FONT_destroy(void *);
extern int s_iMemClass;
void *RCMPAlloc(const char *, int, int, int, int);
int RCMPFree(void *);
extern "C" int MEMCLASS_getfree();
extern "C" int MEMCLASS_create(int index, const char *name, void *address, int size, int arg4, int alignment, int arg6, bool sentinel, bool named, bool locked);
extern "C" int MEMCLASS_remove(int index);
void TLT_ForceLoadFileNormalLocked(bool);
void ClearTimer();

namespace RCMP {
class VD_SUBTITLE {
public:
    ~VD_SUBTITLE();
    static void operator delete(void *p) { rcmp_sys.release(p); }
};
class AV_SUBTITLE_ARRAY {
public:
    ~AV_SUBTITLE_ARRAY();
    static void operator delete(void *p) { rcmp_sys.release(p); }
};
}

class CMoviePlayer {
public:
    CMoviePlayer();
    ~CMoviePlayer();
    static void InitClass();
    void SetMemory(void *memory, int size);
    void CleanMemory();
    int WhichLanguageColumn(int language);
    void SetSpeed(float speed);
    void SetupSubtitle(bool enable, char *textFile, char *fontFile, char *unknownFile, char *indexFile, int language);
    bool SetupSubtitle();
    bool Play(char *name, bool (*callback)(CMoviePlayer *));
    bool PlayMovie(void *memory, int size, char *name, bool (*callback)(CMoviePlayer *));

    int unknown0;
    int subtitlesRequested;
    bool subtitlesEnabled;
    char subtitleTextFile[64];
    char subtitleFontFile[64];
    char subtitleUnknownFile[64];
    char subtitleIndexFile[64];
    int language;
    int unknown10c;
    char *name;
    void *subtitleText;
    int unknown11c;
    void *subtitleFontData;
    void *subtitleFont;
    int unknown128;
    bool (*callback)(CMoviePlayer *);
    RCMP::AV_PLAYER *player;
    RCMP::VIDEO_DRIVER *display;
    RCMP::FRAME *frame;
    float time;
    RCMP::AV_SUBTITLE_ARRAY *subtitles;
    RCMP::VD_SUBTITLE *subtitleDisplay;
};

extern CMoviePlayer *g_pMoviePlayer;

void SetupSpecificHeap(void *memory, int size) {
    if (memory) {
        s_iMemClass = MEMCLASS_getfree();
        MEMCLASS_create(s_iMemClass, "RCMP", memory, size, 64, 64, 0, false, false, true);
    } else {
        s_iMemClass = 0;
    }
}

void ReleaseSpecificHeap() {
    if (s_iMemClass) {
        MEMCLASS_remove(s_iMemClass);
        s_iMemClass = 0;
    }
}

void CMoviePlayer::SetMemory(void *memory, int size) {
    SetupSpecificHeap(memory, size);
    name = 0;
    unknown128 = 0;
    callback = 0;
    display = 0;
    player = 0;
    frame = 0;
    subtitleText = 0;
    unknown11c = 0;
    subtitleFont = 0;
    subtitleFontData = 0;
    subtitles = 0;
    subtitleDisplay = 0;
    RCMP::rcmp_sys.allocate = RCMPAlloc;
    RCMP::rcmp_sys.release = RCMPFree;
    RCMP::rcmp_sys.field8 = (void *)s_iMemClass;
}

void CMoviePlayer::CleanMemory() {
    if (display) {
        delete display;
        display = 0;
    }
    if (player) {
        delete player;
        player = 0;
    }
    if (subtitleText) {
        DWI_free(subtitleText);
        subtitleText = 0;
    }
    if (subtitleFont) {
        FONT_destroy(subtitleFont);
        subtitleFont = 0;
    }
    if (subtitleFontData) {
        DWI_free(subtitleFontData);
        subtitleFontData = 0;
    }
    if (subtitleDisplay) {
        delete subtitleDisplay;
        subtitleDisplay = 0;
    }
    if (subtitles) {
        delete subtitles;
        subtitles = 0;
    }
    name = 0;
    unknown128 = 0;
    callback = 0;
    frame = 0;
    ReleaseSpecificHeap();
}

int CMoviePlayer::WhichLanguageColumn(int language) {
    switch (language) {
    case 0:
    case 1:
        return 0;
    case 5:
        return 1;
    case 4:
        return 2;
    case 3:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    case 8:
        return 8;
    case 2:
        return 4;
    }
    return 0;
}
