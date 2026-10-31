// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptMessageMap.h"

int BSMessageGetMemoryRequirements();
void *BSUtilGetMemory(int, int);

int BSInitMessage() {
    g_pBSMessageMemory = static_cast<unsigned char *>(
        BSUtilGetMemory(BSMessageGetMemoryRequirements(), 0));
    g_pBSMessageMemoryOffset = 0;
    g_MessageQueueHead = 0;
    g_MessageQueueTail = 0;
    BSInitMessageRegistrationArray();
    BSInitMessageRegistrationFreeList();
    g_message_to_event_map.clear();
    return 0;
}

int BSEndMessage() {
    g_message_to_event_map.clear();
    g_MessageRegistrationArray = 0;
    g_MessageRegistrationFreeList = 0;
    return 0;
}
