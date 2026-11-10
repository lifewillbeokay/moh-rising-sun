// CMCDevice: one memory-card slot; see docs/Frontend.md.
#include "MemoryCardDevice.h"
#include "Objectives.h"
#include <stdio.h>
const char *CMCDevice::GetName() const { return name; }
int CMCDevice::GetIndex() const { return this - gs_cards; }
void CMCDevice::Invalidate(bool removed) {
    justInserted = false;
    justRemoved = removed;
    status = 1;
}
