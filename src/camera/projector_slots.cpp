// AI-assisted reconstruction of the projector's type and destroy slots; see docs/Rendering.md.
#include "Projector.h"
int CProjector::GetType() { return type; }
void CProjector::Destroy() { Destroy(this); }
