// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
extern "C" int rand();
// Descriptive placement of the original four-byte scaling literal.
static const float randomScale[1]
    __attribute__((section(".rodata.random_scale"), aligned(4))) = { 1.0f / 2147483648.0f };
float MathFunRandomReal(float min, float max) {
    float range = max - min;
    return min + (range * randomScale[0]) * rand();
}
