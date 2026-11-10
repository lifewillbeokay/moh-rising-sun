// AI-assisted reconstruction of the projector's detach; see docs/Rendering.md.
#include "Projector.h"
int CProjector::Detach() {
    Assign(0);
    attachMatrix = 0;
    return 1;
}
