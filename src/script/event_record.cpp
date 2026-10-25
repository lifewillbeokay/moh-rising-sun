// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptEvents.h"
BSEventRecord_struct *BSEventGetFreeEventRecord() {
    BSEventRecord_struct *record = g_BSEventModule.freeHead;
    g_BSEventModule.freeHead = record->next;
    return record;
}
