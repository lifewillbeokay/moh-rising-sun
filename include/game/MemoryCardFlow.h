// Shared globals and allocation boundary from MemoryCard.cpp. Storage remains original.
#ifndef GAME_MEMORY_CARD_FLOW_H
#define GAME_MEMORY_CARD_FLOW_H
#include "MemoryCardDevice.h"
#include "SaveGamePopups.h"
#include "CRC.h"
extern unsigned char *g_pScratchpad;
extern unsigned int g_uGameDataSize;
extern int g_bMinimalInteraction, g_bWaitingForReply, g_bIsBusy;
extern void *g_pTextDatabase, *g_pMiscTextDatabase;
extern "C" void THREAD_yield(int);
#endif
