// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"
extern int g_bInMultiplayerMode;
extern void BotEvent(BSObject *, unsigned short, void *);

int BSObjectTriggerEvent(BSObject *object, unsigned short event, void *context, BSObject *sender, bool immediate) {
    if (g_bInMultiplayerMode && object && object->scriptClass && object->scriptClass->field_48 == 0x0a987c11)
        BotEvent(object, event, context);
    if (!object) return 0;
    int count = object->scriptClass->levelCount;
    BSMachineThread_struct *thread = object->threads;
    int handled = 0;
    if (object->queueIdentity != ~0U && (object->field_28 || !immediate || event == 41)) {
        for (int i = 0; i < count; ++i, ++thread) {
            BSCode_EventHandlerEntry_struct *handler;
            unsigned short state = BSEventFindDeclaredState(event, thread, &handler);
            if (BSEventFindAnyEventHandler(event, object->scriptClass)) {
                handled = 1;
                if (immediate) BSEventQueueImmediateEvent(object, thread, handler, state, context, sender, event);
                else BSEventTriggerEvent(object, thread, handler, state, context, sender, event);
            }
        }
        if (immediate) BSEventProcessImmediateEvents();
    }
    if (BSEventIsEventMemoryBlock(context) && !handled) BSEventCheckMemBlockReferenceCount(context);
    if (event == 41 && !handled) object->field_28 = 1;
    return handled;
}
