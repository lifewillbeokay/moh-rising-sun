// FEEvent.cpp and FEButtonMap.cpp: small resizable arrays.
#include "FEngLists.h"
#include "FEngStrings.h"
#include <string.h>
struct FEEvent { unsigned long data[3]; };
class FEEventList {
  public:
    long count;      // +0x0
    FEEvent *events; // +0x4
    void operator=(FEEventList &);
    void SetCount(long);
};
extern "C" void *DWI_alloc(const char *, int, int);

void FEButtonMap::SetCount(unsigned long newCount) {
    if (buttons) delete[] buttons;
    buttons = 0;
    if (newCount) buttons = (FEObject **)DWI_alloc("source/FEngine/FEButtonMap.cpp:40", newCount * 4, 256);
    count = newCount;
}
