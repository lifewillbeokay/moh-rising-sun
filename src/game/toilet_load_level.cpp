// A fragment of the original toilet.cpp: the first states of the asynchronous level load, each run by
// TLT_Async_Load. Names come from the symbol table. Each state returns how far to advance. The file
// name strings are the file's private statics, reached here as externals.
#include "Toilet.h"
#include <stdio.h>

void TLT_LoadFile_Async_Start(const char *, int);
bool TLT_LoadFile_Async_Load(char **, int *);

extern char *szTLT_LevelRtcFileName;
extern char *szTLT_LevelBigFileName;

static int TLT_Load_Begin() { return 1; }

static int TLT_Load_StartRelo() {
    char path[128];
    sprintf(path, "data\\%d\\%d_%d\\%s", g_toilet.mission, g_toilet.mission, g_toilet.stage, szTLT_LevelRtcFileName);
    TLT_LoadFile_Async_Start(path, 0);
    return 1;
}

static int TLT_Load_WaitRelo() {
    char *buffer = 0;
    int size = 0;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        g_toilet.relocationBufferSize = size;
        g_toilet.relocationBuffer = buffer;
        g_toilet.rtcFile.SetArchiveBuffer(buffer, operator delete);
        g_toilet.rtcFile.Dearchive();
        return 1;
    }
    return 0;
}

static int TLT_Load_StartDebug() { return 1; }

static int TLT_Load_WaitDebug() { return 1; }

static int TLT_Load_StartLevel() {
    char path[128];
    sprintf(path, "data\\%d\\%d_%d\\%s", g_toilet.mission, g_toilet.mission, g_toilet.stage, szTLT_LevelBigFileName);
    TLT_LoadFile_Async_Start(path, 0);
    return 1;
}

static int TLT_Load_WaitLevel() {
    char *buffer;
    int size;
    if (!TLT_LoadFile_Async_Load(&buffer, &size)) {
        g_toilet.levelBigFile = buffer;
        g_toilet.levelBigFileSize = size;
        return 1;
    }
    return 0;
}
