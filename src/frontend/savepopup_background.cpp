// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
extern int g_bBackgroundEnabled;
void CSaveGamePopup::EnableBackground() { g_bBackgroundEnabled = 1; }
void CSaveGamePopup::DisableBackground() { g_bBackgroundEnabled = 0; }
int CSaveGamePopup::IsBackgroundEnabled() { return g_bBackgroundEnabled; }
