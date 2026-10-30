// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
extern int g_iDefaultStackSize;
int BSMachineGetMemoryRequirements() {
    return g_iDefaultStackSize * 4 + g_iBSMessageRegistrationListSize * sizeof(BSMessageListView);
}
