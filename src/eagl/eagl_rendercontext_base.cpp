// AI-assisted reconstruction of EAGL's render-context base destructor; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
RenderContextBase::~RenderContextBase() {
    while (privateData->firstViewPort)
        DeleteViewPort(privateData->firstViewPort);
}
