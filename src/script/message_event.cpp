// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptMessageMap.h"

void BSRegisterMessageToEvent(unsigned short event, unsigned short message, BSObject *object) {
    BSMessageToEventRecord record;
    record.object = object;
    record.event = event;
    g_message_to_event_map.insert(_STL::make_pair(message, record));
}

void BSUnregisterMessageToEvent(unsigned short event, unsigned short message, BSObject *object) {
    BSMessageEventMap::iterator it;
    it = g_message_to_event_map.find(message);
    if (it != g_message_to_event_map.end()) {
        // Preserve the original scan: it does not recheck end() after increment.
        while (it->first == message) {
            if (it->second.event == event && it->second.object == object) {
                g_message_to_event_map.erase(it);
                break;
            }
            ++it;
        }
    }
}
