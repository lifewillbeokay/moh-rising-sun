#ifndef GAME_SCRIPT_MESSAGE_MAP_H
#define GAME_SCRIPT_MESSAGE_MAP_H

#include "profile.h"
#include <map>
#include "ContainerTypes.h"
#include "ScriptRuntime.h"

// The retained STLport profile and independently recovered message record.
// Field names and storage limits are documented in docs/Script.md.
typedef _STL::multimap<unsigned short, BSMessageToEventRecord> BSMessageEventMap;
extern BSMessageEventMap g_message_to_event_map;
extern unsigned char *g_pBSMessageMemory;
extern int g_pBSMessageMemoryOffset;
int BSInitMessageRegistrationArray();
int BSInitMessageRegistrationFreeList();

#endif
