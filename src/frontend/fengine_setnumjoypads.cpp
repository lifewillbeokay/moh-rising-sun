// FEngine.cpp: queued messages and package commands.
#include "FEngObjects.h"
#include "FEngStrings.h"
#include <string.h>
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
class FEListBox { public: void ScrollSelection(long, long); };
class FECodeListBox { public: void ScrollSelection(long, long); };
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FEngine::SetNumJoyPads(unsigned char count) {
    if (count != joyPadCount) {
        if (joyPads) delete[] joyPads;
        if (count) joyPads = new ("source/FEngine/FEngine.cpp:160") FEJoyPad[count];
        joyPadCount = count;
        memset(joyPadState, 0, 76);
        activeMask = 0;
        for (int i = 0; i < count; i++) activeMask |= 1 << i;
    }
}
