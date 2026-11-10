// AI-assisted reconstruction from GR8E69. Fragments of the original BotMan source: function and
// variable names come from the symbol table, member names are inferred.
#include "AIBot.h"
#include "AISplinePath.h"
#include "TriggerObject.h"
extern "C" int DWI_free(void *);

extern int g_NumBotsCreated;
extern void *g_pBotMem;
extern char *g_pBots[];
static int s_BotMemSize;
void DestroyBots();
void InitBotAreaData();
void CleanBotAreaData();
void FindJumpConnections2();
void InitAreaEdges();
void ShrinkAreaMesh();

void BotSetup() {
    if (g_NumBotsCreated > 0) {
        g_BotMan.resetFlag = 0;
        InitBotAreaData();
        g_BotMan.SetupCampingPoints();
        FindJumpConnections2();
        InitAreaEdges();
        ShrinkAreaMesh();
    }
}

void BotClean() {
    CleanBotAreaData();
    DestroyBots();
    if (g_pBotMem) {
        DWI_free(g_pBotMem);
        g_pBotMem = 0;
    }
}

void BotRestart() {
    BotClean();
    g_NumBotsCreated = 0;
    g_BotMan.clock = 0.0f;
}

void *GetBotMem() {
    int size = 11568;
    s_BotMemSize = size;
    char *mem = (char *)g_pBotMem + g_NumBotsCreated * size;
    g_pBots[g_NumBotsCreated] = mem;
    return mem;
}
