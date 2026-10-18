#ifndef GAME_MATHFUN_H
#define GAME_MATHFUN_H

// AI-assisted reconstruction of the original float/int template instances.
// The floating comparison also preserves the original unordered-input path.
template <class T> T MathFunClamp(T value, T min, T max) {
    if (value < min) {
        return min;
    } else {
        T result = value;
        if (!(result <= max)) {
            result = max;
        }
        return result;
    }
}
float MathFunRandomReal(float, float);
class CVector3;
float MathFunGetPanAngleDiffNoRoll(CVector3 *, CVector3 *, CVector3 *);
#endif
