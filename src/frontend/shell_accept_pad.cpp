// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#include "ShellManager.h"
signed char ShellManager::GetIndexPadLastPressedAccept() {
    return engine ? engine->lastAcceptPad : -1;
}
