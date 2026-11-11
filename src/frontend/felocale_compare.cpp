// AI-assisted GR8E69 reconstruction; private FELocalizer.cpp helpers.
#ifdef DECOMP_COMPARE
// See docs/research/feng-packages.md.
int FEUpperCase(char);
static int CompareUpper(const short *wide, const char *text) {
    for (;;) {
        if ((unsigned short)(*wide + 128) > 254) return *text - *wide;
        int difference = FEUpperCase(*text) - FEUpperCase((char)*wide);
        if (difference) return difference;
        if (!*wide++ || !*text++) return 0;
    }
}
static int CompareUpperSubst(const short *wide, const char *text) {
    for (;;) {
        if ((unsigned short)(*wide + 128) > 254) return *text ? *text - *wide : 0;
        if (!*text) return 0;
        int difference = FEUpperCase(*text) - FEUpperCase((char)*wide);
        if (difference) return difference;
        if (!*wide++ || !*text++) return 0;
    }
}
static int WideToCharString(char *out, const short *wide) {
    if (!wide || !out) return 0;
    while (*wide) {
        if ((unsigned short)(*wide + 128) > 255) return 0;
        *out++ = *wide++;
    }
    *out = 0;
    return 1;
}
#endif
