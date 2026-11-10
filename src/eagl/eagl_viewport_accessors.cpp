// AI-assisted reconstruction of EAGL viewport accessors; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
inline void ViewPort::operator delete(void *p, unsigned int n) { EAGLFree(p, n); }
void ViewPort::SetEnableModelSphereCull(int enable) {
    modelSphereCull = enable;
}
int ViewPort::GetEnableModelSphereCull() {
    return modelSphereCull;
}
ViewPort::~ViewPort() {}
int ViewPort::GetProjectionType() const {
    return privateData.projectionType;
}
void ViewPort::SetBackgroundColour(Colour colour) {
    privateData.backgroundColour = colour;
}
