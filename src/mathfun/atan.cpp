// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
extern "C" float atanf(float);
float MathFunAtan2F(float y,float x) {
    if (!(x <= 0.0f)) return atanf(-y/x);
    if (!(x >= 0.0f)) {
        if (!(y >= 0.0f)) return 3.141593f-atanf(y/x);
        else return -3.141593f-atanf(y/x);
    }
    if (y <= 0.0f) return 1.5707965f;
    else return -1.5707965f;
}
