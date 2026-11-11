// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEScript.h"
void FEPackage::IssueScriptMessages(FEngine *engine, FEObject *object, FEScript *script,
                                   long start, long end) {
    FEEvent *events = script->events.events;
    long count = script->events.count;
    if (start > end) return;
    if ((unsigned long)end == script->unknown_c) ++end;
    long i = 0;
    while (i < count && events[i].time < (unsigned long)start) ++i;
    for (; i < count && events[i].time < (unsigned long)end; ++i) {
        FEEvent &event = events[i];
        switch (event.target) {
        case 0xffffffff: engine->SendMessageToGame(event.message, object, this, 0); break;
        case 0xfffffffa: case 0xfffffffb: case 0xfffffffc:
            engine->QueueMessage(event.message, object, this, (FEObject *)event.target, 0); break;
        case 0:
            if (event.message != 0x1b3909aa) engine->QueueMessage(event.message, object, this, 0, 0);
            else SetCurrentButton(engine, FindObjectByGUID(0), true);
            break;
        default: {
            FEObject *target = FindObjectByGUID(event.target);
            if (event.message == 0x1b3909aa) SetCurrentButton(engine, FindObjectByGUID(event.target), true);
            else if (object) engine->QueueMessage(event.message, object, this, target, 0);
            break;
        }
        }
    }
}
#endif
