// AI-assisted reconstruction of EAGL viewport extension and restart; see docs/Rendering.md.
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
void ViewPort::SetGuardBandScale(float) {}
ViewPortExtension::ViewPortExtension(ViewPort *viewPort) : owner(viewPort) {}
ViewPortExtension::~ViewPortExtension() {}
void ViewPortPrivate::ReBegin() {
    owner->EndView();
    owner->BeginView();
}
