// AI-assisted reconstruction from GR8E69; see docs/Memory.md.
#include "MemStack.h"
MemStack::MemStack(int size) {
    base = (unsigned char *)DWI_alloc("source/system/sys_memstack.cpp:7", size, 1024);
    cursor = base;
    capacity = size;
}
MemStack::~MemStack() {
    delete[] base;
}
