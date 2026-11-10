// ShellManager::PerformPostDrawUpdates; see docs/Frontend.md.
#include "ShellHandlerMap.h"
void ShellManager::PerformPostDrawUpdates() {
    ShellHandlerMap::iterator it = handlers.begin();
    ShellHandlerMap::iterator end = handlers.end();
    for (; it != end; ++it) {
        (*it).second->HandleTick();
    }
}
