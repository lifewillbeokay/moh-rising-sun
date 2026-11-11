// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include <string.h>
int FEngine::RecordPackageMarker(const char *name) {
    if (markerCount == 16) return 0;
    strcpy(markers[markerCount++], name);
    return 1;
}
#endif
