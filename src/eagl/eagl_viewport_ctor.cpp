// AI-assisted reconstruction of EAGL's viewport constructor; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
extern MATRIX4 *identity;
ViewPort::ViewPort(RenderContextBase *context) : extension(this), privateData(this) {
    previous = 0;
    modelSphereCull = 0;
    privateData.modelMatrix = *identity;
    privateData.viewMatrix = *identity;
    privateData.viewProjectionMatrix = *identity;
    privateData.context = context;
}
