// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

void AnimShape::SetStateByCRC(int crc, EState state) {
    AnimShape *shape = FindFromCRC(crc);
    if (shape)
        shape->state = state;
}
void AnimShape::SetFrameByCRC(int crc, float frame) {
    AnimShape *shape = FindFromCRC(crc);
    if (shape)
        shape->SetFrame(frame);
}
void AnimShape::SetFrameRateByCRC(int crc, float rate) {
    AnimShape *shape = FindFromCRC(crc);
    if (shape)
        shape->frameRate = rate;
}
