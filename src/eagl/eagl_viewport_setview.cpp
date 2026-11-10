// AI-assisted reconstruction of EAGL viewport view control; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
void ViewPort::SetViewMatrix(const MATRIX4 &matrix) {
    privateData.viewMatrix = matrix;
    if (privateData.active)
        privateData.ReBegin();
}
