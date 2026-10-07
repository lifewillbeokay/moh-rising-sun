// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
extern "C" int rand();
int MathFunRandomSign();
float MathFunRandomRealSigned(float min,float max) {
    if(min==max)return min;
    float range=max-min;
    float value=min+(range*(1.0f/2147483648.0f))*rand();
    return value*MathFunRandomSign();
}
