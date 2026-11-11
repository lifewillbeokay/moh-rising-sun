// AI-assisted GR8E69 reconstruction; private FELocalizer.cpp helpers.
#ifdef DECOMP_COMPARE
// See docs/research/feng-packages.md.
static int AtoI(const char *text) {
    int value = 0;
    if (!text) return 0;
    while (*text && (unsigned long)(*text - '0') < 10) value = value * 10 + *text++ - '0';
    return value;
}
static char *ItoA(char *out, unsigned long value) {
    unsigned long power = 1;
    while (power <= value) power *= 10;
    while ((power /= 10) != 0) {
        *out++ = value / power + '0';
        value %= power;
    }
    *out = 0;
    return out;
}
#endif
