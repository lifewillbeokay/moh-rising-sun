// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"

void BSEventCurrentlyProcessingMessages(bool);
void BSMessageProcessSingleMessageInQueue();
int BSEventProcessImmediateEvents();

int BSProcessMessages(int limit) {
    int i = 0;
    int count = g_MessageQueueTail - g_MessageQueueHead;
    if (count < 0) count += 1024;
    if (count > limit) count = limit;
    while (i < count) {
        ++i;
        BSEventCurrentlyProcessingMessages(true);
        BSMessageProcessSingleMessageInQueue();
        BSEventCurrentlyProcessingMessages(false);
        BSEventProcessImmediateEvents();
    }
    return 0;
}
