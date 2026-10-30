// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
extern BSClass_struct *g_pcClassList;
void BSFileDeleteClass(BSClass_struct *);
int BSEndMachine() {
    BSClass_struct *current = g_pcClassList;
    while (current) {
        BSClass_struct *next = current->next;
        BSFileDeleteClass(current);
        current = next;
    }
    g_pcClassList = 0;
    return 0;
}
