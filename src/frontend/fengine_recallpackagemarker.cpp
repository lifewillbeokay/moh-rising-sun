// FEngine.cpp: queued messages and package commands.
#include "FEngObjects.h"
#include "FEngStrings.h"
#include <string.h>
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
class FEListBox { public: void ScrollSelection(long, long); };
class FECodeListBox { public: void ScrollSelection(long, long); };
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

char *FEngine::RecallPackageMarker() {
    if (markerCount == 0) return 0;
    return markers[--markerCount];
}
