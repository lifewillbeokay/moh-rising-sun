// AI-assisted GR8E69 reconstruction; private FEObject.cpp helper.
#ifdef DECOMP_COMPARE
// See docs/research/feng-packages.md.
static bool Close(float a, float b, float tolerance) {
    if (a + tolerance < b) return false;
    return a - tolerance <= b;
}
#endif
