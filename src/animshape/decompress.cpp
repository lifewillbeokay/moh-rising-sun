// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

void AnimShape::Decompress(int index) {
    char *base = reinterpret_cast<char *>(shapeFile);
    pendingShape = reinterpret_cast<SHAPE *>(base + *reinterpret_cast<unsigned int *>(base + 20 + index * 8));
    dirty = 1;
}
