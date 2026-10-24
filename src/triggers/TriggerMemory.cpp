// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "TriggerObject.h"
#include "ScriptRuntime.h"
extern unsigned int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
extern int BSFileGetClass(const char *, BSClass_struct **);

int GetTriggerBSObjectMemUsage(int first) {
    TriggerObject *object = g_pTriggerObjects + first;
    int total = 0;
    for (unsigned int i = 0; i < g_pNumTriggers - first; ++i, ++object) {
        const char *script = object->GetScript();
        if (script && *script) total += 48;
    }
    return total;
}
int GetTriggerScriptMemUsage(int first) {
    TriggerObject *object = g_pTriggerObjects + first;
    int total = 0;
    for (unsigned int i = 0; i < g_pNumTriggers - first; ++i, ++object) {
        const char *script = object->GetScript();
        if (script && *script) {
            BSClass_struct *scriptClass;
            BSFileGetClass(script, &scriptClass);
            total += scriptClass->instanceMemorySize;
        }
    }
    return total;
}
