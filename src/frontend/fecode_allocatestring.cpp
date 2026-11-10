// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

short *FECodeListBox::AllocateString() {
    short *text = strings[stringsInUse++];
    text[0] = 0;
    return text;
}
void FECodeListBox::DeallocateString(short *text) { strings[--stringsInUse] = text; }
