// A fragment of the original input code: the input manager's constructor. Names as in Shell.h.
#include "Shell.h"

CInputManager::CInputManager() {
    for (int i = 0; i < 4; i++) {
        portKeymaps[i] = 0;
        controllers[i] = 0;
    }
    controllerChecksEnabled = 1;
    field2400 = -1;
    field2404 = 0;
    badControllerSet = 0;
    field2408 = -1;
    badControllerPort = -1;
}
