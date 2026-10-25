// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"

int BSEventFindAnyEventHandler(unsigned short event, BSClass_struct *scriptClass) {
    int mask = 1 << (event & 31);
    if (scriptClass->eventMask[event >> 5] & mask) return 1;
    return 0;
}
unsigned short BSEventFindDeclaredState(unsigned short event, BSMachineThread_struct *thread, BSCode_EventHandlerEntry_struct **handler) {
    BSStateView **states = thread->level->states;
    unsigned short state = thread->stateId;
    while (state != 0xffff) {
        BSStateView *entry = states[state];
        BSCode_EventHandlerEntry_struct *candidate = entry->events;
        for (int i = 0; i < entry->eventCount; ++i, ++candidate) {
            if (candidate->flags & 4) { *handler = 0; return 0xffff; }
            if (candidate->eventNumber == event) {
                if (candidate->flags & 1) {
                    *handler = candidate;
                    return state;
                }
                *handler = 0;
                return 0xffff;
            }
            if (candidate->eventNumber > event) break;
        }
        state = states[state]->parent;
    }
    *handler = 0;
    return 0xffff;
}
