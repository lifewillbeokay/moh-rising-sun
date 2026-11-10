// AI-assisted reconstruction of the projector's frustum accessors; see docs/Rendering.md.
#include "Projector.h"
void CProjector::SetFrustum(CVolFrustum &f) { frustum = f; }
void CProjector::SetMatrix(CMatrix &m) { projection = m; }
void CProjector::SetRadius(float r) { radius = r; }
const CVolFrustum &CProjector::GetFrustum() const { return frustum; }
CProjector *CProjector::AsProjector() { return this; }
const CProjector *CProjector::AsProjector() const { return this; }
