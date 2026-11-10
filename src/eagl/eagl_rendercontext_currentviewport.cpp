// AI-assisted reconstruction of EAGL's viewport list; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
ViewPort *RenderContextBase::GetCurrentViewPort() const {
    return privateData->currentViewPort;
}
void RenderContextPrivateBase::SetCurrentViewPort(ViewPort *viewPort) {
    currentViewPort = viewPort;
}
