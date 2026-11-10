// AI-assisted reconstruction of EAGL's viewport list; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
inline void *ViewPort::operator new(unsigned int n) { return EAGLMalloc(n, 0); }
ViewPort *RenderContextBase::NewViewPort() {
    ViewPort *viewPort = new ViewPort(this);
    viewPort->privateData.next = privateData->firstViewPort;
    privateData->firstViewPort = viewPort;
    return viewPort;
}
