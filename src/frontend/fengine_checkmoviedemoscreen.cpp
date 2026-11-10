// FEngine.cpp: queued messages and package commands.
#include "FEngObjects.h"
#include "FEngStrings.h"
#include <string.h>
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
class FEListBox { public: void ScrollSelection(long, long); };
class FECodeListBox { public: void ScrollSelection(long, long); };
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FEngine::CheckMovieDemoScreen(char *name) {
    if (strcmp(name, "Single_Multiplayer_selection.fng") == 0) checkMovieDemoScreen = 1;
    if (strcmp(name, "MainMenu.fng") == 0) checkMovieDemoScreen = 1;
}
