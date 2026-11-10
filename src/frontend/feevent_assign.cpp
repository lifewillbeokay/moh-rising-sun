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

void FEEventList::operator=(FEEventList &other) {
    SetCount(other.count);
    { void *to = events; void *from = other.events; memcpy(to, from, count * sizeof(FEEvent)); }
}
