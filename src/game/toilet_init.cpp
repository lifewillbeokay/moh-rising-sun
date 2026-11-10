// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the file holding Toilet's loading functions; the image records no name for that file.
#include "Toilet.h"

void TLT_InitLang();
int TLT_Async_LoadSystem();

typedef int (*ToiletLoadState)();
extern ToiletLoadState g_ToiletLoadStates[];

void TLT_Init(int mission, int stage) {
    g_toilet.mission = mission;
    g_toilet.stage = stage;
    TLT_InitLang();
    TLT_Async_LoadSystem();
}

void TLT_Async_Start() {
    g_toilet.asyncState = 0;
    g_toilet.asyncPercent = 0.0f;
    g_toilet.loadFileNormalLocked = 1;
}

int TLT_Async_Load() {
    int state = g_toilet.asyncState;
    int stateCount = 20;
    if (state >= stateCount)
        return 0;
    int advance = g_ToiletLoadStates[state]();
    g_toilet.asyncState = advance + state;
    int mission = g_toilet.mission;
    int extra = 2;
    if ((unsigned)(mission - 2) <= 2)
        extra = 1;
    g_toilet.asyncPercent = (float)(state + g_toilet.soundBankIndex) / (float)(extra + stateCount);
    return 1;
}
