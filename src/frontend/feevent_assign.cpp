// FEEvent.cpp and FEButtonMap.cpp: small resizable arrays.
#include "FEngLists.h"
#include "FEngStrings.h"
#include <string.h>
#include "FEEvent.h"
extern "C" void *DWI_alloc(const char *, int, int);

void FEEventList::operator=(FEEventList &other) {
    SetCount(other.count);
    { void *to = events; void *from = other.events; memcpy(to, from, count * sizeof(FEEvent)); }
}
