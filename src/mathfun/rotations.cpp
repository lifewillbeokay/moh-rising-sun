// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
extern "C" float sinf(float);
extern "C" float cosf(float);
void MathFunRotateAboutY(float* x,float* z,float a) {
    float s=sinf(a),c=cosf(a);
    float oldX=*x;
    float oldZ=*z;
    *z=oldZ*c-oldX*s;
    *x=oldZ*s+oldX*c;
}
void MathFunRotateAboutZ(float* x,float* y,float a) {
    float s=sinf(a),c=cosf(a);
    float oldX=*x;
    float oldY=*y;
    *x=oldX*c-oldY*s;
    *y=oldX*s+oldY*c;
}
