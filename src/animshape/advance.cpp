// AI-assisted reconstruction from GR8E69; see docs/research/broad-source-sweep.md.
#include "AnimShape.h"
template <class T> T MathFunClamp(T, T, T);

#ifdef DECOMP_COMPARE
void AnimShape::AdvanceFrames(float amount) {
    float value = frame;
    switch ((signed char)state) {
    case StateL:
    case StateO:
    case StateP:
        value += amount;
        if (!(value >= 0.0f))
            value = 0.0f;
        break;
    case Statel:
    case Stateo:
    case Statep:
        value -= amount;
        if (!(value < (float)frameCount))
            value = (float)frameCount - 1.0f;
        break;
    }
    while (value < 0.0f || value >= (float)frameCount) {
        switch ((signed char)state) {
        case StateO:
            if (!(value < (float)frameCount)) {
                value = (float)frameCount - 1.0f;
                state = StateDefault;
            }
            break;
        case Stateo:
            if (!(value >= 0.0f)) {
                value = 0.0f;
                state = StateDefault;
            }
            break;
        case StateL:
            if (!(value < (float)frameCount))
                value -= (float)frameCount;
            break;
        case Statel:
            if (!(value >= 0.0f))
                value += (float)frameCount;
            break;
        case StateP:
            if (!(value < (float)frameCount)) {
                value = (float)frameCount - (value - (float)frameCount) - 1.0f;
                state = Statep;
            }
            break;
        case Statep:
            if (!(value >= 0.0f)) {
                value = -value;
                state = StateP;
            }
            break;
        default:
            value = MathFunClamp(value, 0.0f, (float)frameCount - 1.0f);
            break;
        }
    }
    if ((int)value != (int)frame)
        Decompress((int)value);
    frame = value;
}
#endif
