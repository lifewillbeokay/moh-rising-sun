// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
extern "C" int strcasecmp(const char *, const char *);
extern BSClass_struct *g_pcClassList;

BSClass_struct *GetClassFromDatabase(const char *name) {
    BSClass_struct *current = g_pcClassList;
    while (current) {
        if (!strcasecmp(name, current->name)) return current;
        current = current->next;
    }
    return 0;
}

int AddClassToDatabase(BSClass_struct *scriptClass) {
    scriptClass->next = g_pcClassList;
    g_pcClassList = scriptClass;
    return 0;
}
