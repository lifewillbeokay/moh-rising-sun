// AI-assisted reconstruction from GR8E69; see docs/Geometry.md.
extern "C" float sinf(float),cosf(float);
void MathLLAngleInit() {
}
void MathSinCos(int angle,float* sine,float* cosine) {
    float radians=angle*(6.2831853071795864769f/16777216.0f);
    *sine=sinf(radians);
    *cosine=cosf(radians);
}
