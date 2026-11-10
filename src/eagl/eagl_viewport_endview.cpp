// AI-assisted reconstruction of EAGL viewport view control; see docs/Rendering.md.
#pragma implementation "EAGLViewPort.h"
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
inline void ViewPort::EndView() {
    privateData.context->privateData->SetCurrentViewPort(0);
    ViewPort *view = previous;
    privateData.active = 0;
    if ((unsigned int)view > 1)
        view->BeginView();
    previous = 0;
}
