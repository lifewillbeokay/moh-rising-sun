// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

void AnimShape::AdvanceByCRC(int crc) {
    AnimShape *shape = FindFromCRC(crc);
    if (shape)
        shape->StepToNextFrame();
}
void AnimShape::AddToGlobalList() {
    next = ms_pShapes;
    ms_pShapes = this;
}
void AnimShape::RemoveFromGlobalList() {
    AnimShape **link = &ms_pShapes;
    while (*link && *link != this)
        link = &(*link)->next;
    if (*link) {
        *link = next;
        next = 0;
    }
}
