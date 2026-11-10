// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

FECodeListBox::~FECodeListBox() {
    if (cells) delete[] cells;
    if (stringBlock) delete[] stringBlock;
    if (strings) delete[] strings;
}
