// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "ScriptStartup.h"
#include "TriggerObject.h"
#include "FlexPropDatabase.h"
#include "Light.h"
#include "ScriptTimers.h"
extern unsigned int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];
int GetTriggerScriptMemUsage(int);
int GetTriggerBSObjectMemUsage(int);

void InitAllTriggers() {
    int firstFlex = g_pNumTriggers;
    TriggerObject *object = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; ++i, ++object) {
        xyzProperty_Struct *record = object->properties.legacy;
        object->flagBits = 0;
        object->scriptObject = 0;
        if (record->field0c == 13 && !((MOH_core_Struct *)record)->field40)
            CLight::Create(record);
    }
    object = g_pTriggerObjects + firstFlex;
    TriggerObject *newObjects = object;
    g_pNumTriggers += FlexPropDatabase::GetNumProperties();
    FlexProp property;
    for (int i = 0; i < FlexPropDatabase::GetNumProperties(); ++i, ++object) {
        FlexPropDatabase::GetPropertyByIndex(i, property);
        object->properties.flex = property;
        object->flagBits = 0x40000000;
        if (object->GetNativeBindingType() == 5) {
            if (object->GetChildType() == 0x5b607813)
                object->flagBits = (object->flagBits | 0x10000000) & 0xff000000;
        } else if (property.IsA("BuddyLeadCheckpoint")) {
            object->flagBits |= 0x08000000;
        }
    }
    BSUtilSetScriptingSystemState(SCRIPT_SYSTEM_STARTUP);
    BSUtilInitStartupAllocator(GetTriggerScriptMemUsage(firstFlex));
    BSInitBSObjectStartupAllocator(GetTriggerBSObjectMemUsage(firstFlex));
    object = newObjects;
    for (int i = 0; i < FlexPropDatabase::GetNumProperties(); ++i, ++object) {
        const char *script = object->GetScript();
        if (!script || !*script)
            object->scriptObject = 0;
        else
            CreateBSObject(script, &object->scriptObject, 0, object, 0);
    }
    BSUtilSetScriptingSystemState(SCRIPT_SYSTEM_RUNNING);
    object = newObjects;
    for (int i = 0; i < FlexPropDatabase::GetNumProperties(); ++i, ++object) {
        const char *script = object->GetScript();
        if (script && *script)
            BSObjectTriggerEvent(object->scriptObject, 41, 0, 0, true);
    }
}
