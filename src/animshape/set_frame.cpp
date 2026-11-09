// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"

void AnimShape::SetFrame(float value) {
    if (!(value >= 0.0))
        value = 0.0f;
    if (!(value <= (float)frameCount))
        value = (float)frameCount;
    if (value != frame)
        Decompress((int)value);
    frame = value;
}
