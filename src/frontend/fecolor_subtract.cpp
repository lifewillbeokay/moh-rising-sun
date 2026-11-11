// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEColor.h"
FEColor FEColor::operator-(const FEColor &other) const {
    FEColor result;
    result.b = b - other.b; result.g = g - other.g;
    result.r = r - other.r; result.a = a - other.a;
    return result;
}
#endif
