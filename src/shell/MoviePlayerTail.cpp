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
void SetupSpecificHeap(void *, int);
void ReleaseSpecificHeap();
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

void CMoviePlayer::SetSpeed(float) {}

void CMoviePlayer::InitClass() {
    RCMP::rcmp_sys.SetREALDefaults();
}

CMoviePlayer::CMoviePlayer() {
    subtitlesRequested = 0;
    unknown0 = 1;
    g_pMoviePlayer = this;
}

CMoviePlayer::~CMoviePlayer() {
    g_pMoviePlayer = 0;
}

void CMoviePlayer::SetupSubtitle(bool enable, char *textFile, char *fontFile, char *unknownFile, char *indexFile, int language) {
    subtitlesEnabled = enable;
    sprintf(subtitleTextFile, "%s", textFile);
    char *font = subtitleFontFile;
    sprintf(font, "%s", fontFile);
    sprintf(subtitleUnknownFile, "%s", unknownFile);
    sprintf(subtitleIndexFile, "%s", indexFile);
    this->language = language;
    subtitlesRequested = 1;
    const char *fontExtension = "gfn";
    char *extension = strrchr(font, '.');
    if (extension && *extension && *++extension)
        strcpy(extension, fontExtension);
}

bool CMoviePlayer::PlayMovie(void *memory, int size, char *name, bool (*callback)(CMoviePlayer *)) {
    SetMemory(memory, size);
    if (subtitlesRequested) {
        TLT_ForceLoadFileNormalLocked(true);
        SetupSubtitle();
        TLT_ForceLoadFileNormalLocked(false);
    }
    bool result = Play(name, callback);
    CleanMemory();
    ClearTimer();
    return result;
}
