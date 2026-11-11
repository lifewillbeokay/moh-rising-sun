// FEEvent.cpp and FEButtonMap.cpp: small resizable arrays.
#include "FEngLists.h"
#include "FEngStrings.h"
#include <string.h>
#include "FEEvent.h"
extern "C" void *DWI_alloc(const char *, int, int);

void FEButtonMap::SetCount(unsigned long newCount) {
    if (buttons) delete[] buttons;
    buttons = 0;
    if (newCount) buttons = (FEObject **)DWI_alloc("source/FEngine/FEButtonMap.cpp:40", newCount * 4, 256);
    count = newCount;
}
