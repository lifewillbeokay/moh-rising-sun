// AI-assisted reconstruction from GR8E69. Part of the original player.cpp, named by this allocation label.
#include "CameraShake.h"

void *CPlayerObject::operator new(unsigned int size) { return DWI_alloc("source/object/player.cpp:325", size, 1024); }

void CPlayerObject::operator delete(void *memory) { DWI_free(memory); }
