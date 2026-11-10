// FEngine's list and marker helpers; see docs/Frontend.md.
#include "FEngine.h"
#include <string.h>

void FEngine::ClearPackageMarkers() {
    for (unsigned int i = 0; i < 16; i++) markers[i][0] = 0;
    markerCount = 0;
}
