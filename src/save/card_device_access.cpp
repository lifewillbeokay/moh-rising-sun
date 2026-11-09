// AI-assisted reconstruction from GR8E69; names beyond symbols are descriptive.
#include "MemoryCardDevice.h"
#include "Objectives.h"
#include <stdio.h>
const char *CMCDevice::GetName() const { return name; }
int CMCDevice::GetIndex() const { return this - gs_cards; }
