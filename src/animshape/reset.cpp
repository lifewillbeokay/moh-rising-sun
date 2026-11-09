// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

void AnimShape::ResetByCRC(int crc) {
    for (AnimShape *shape = ms_pShapes; shape; shape = shape->next) {
        if (shape->crc == crc) {
            shape->frame = -1.0f;
            shape->updateFrame = -1;
        }
    }
}
