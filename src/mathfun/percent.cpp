// AI-assisted reconstruction from GR8E69; see docs/MathFun.md.
extern "C" int rand();
long long MathFunGetRandomPercent() {
    return ((long long)rand()*100)>>31;
}
int MathFunTestPercent(long long value,long long threshold) {
    if(value<threshold)return 1;
    return 0;
}
