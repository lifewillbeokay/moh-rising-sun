// AI-assisted reconstruction of the projector's transform accessors; see docs/Rendering.md.
#include "Projector.h"
unsigned int CProjector::IsDrawEnabled() const { return 0; }
void CProjector::GetTMLocalToWorld(CMatrix &m) const { m = localToWorld; }
void CProjector::GetPosition(CVector3 &v) const { v = localToWorld.GetPos(); }
void CProjector::GetRightward(CVector3 &v) const { v = localToWorld.GetRight(); }
void CProjector::GetForward(CVector3 &v) const { v = localToWorld.GetFront(); }
void CProjector::GetUpward(CVector3 &v) const { v = localToWorld.GetUp(); }
