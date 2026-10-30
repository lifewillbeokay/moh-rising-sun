// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"
extern int Gameloop_QueryState();

int BSEventProcessSingleEvent(BSEventRecord_struct *record) {
    int result = 0;
    int execute = 0;
    BSCode_EventHandlerEntry_struct *handler = 0;
    BSMachineThread_struct *thread = record->thread;
    if (record->object->queueIdentity != record->objectIdentity) return 0;
    unsigned short state = BSEventFindDeclaredState(record->eventNumber, thread, &handler);
    int gameState = Gameloop_QueryState();
    if (handler && gameState != 4 && gameState != 5) {
        if (state != record->declaredState) execute = (handler->flags >> 1) & 1;
        else execute = 1;
    }
    if (record->eventNumber == 41) record->object->field_28 = 1;
    if (execute) {
        unsigned int code = handler->code;
        thread->stackTop = g_piStackTop + 3;
        thread->frame = g_piStackTop + 2;
        thread->stackTop[-1] = (int)record->context;
        *thread->stackTop = (int)record->sender;
        thread->instruction = thread->level->codeBases[code >> 24] + (code & 0xffffff);
        result = BSExecuteThread(record->object, record->thread);
    }
    return result;
}
