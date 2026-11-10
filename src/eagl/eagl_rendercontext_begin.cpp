// AI-assisted reconstruction of EAGL render-context construction; see docs/Rendering.md.
#include <dolphin/gx/GXManage.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/gx/GXTexture.h>
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
inline RenderContextBase::RenderContextBase(RenderContextExtension *e, RenderContextPrivateBase *p)
    : extension(e), unknown_4(0), privateData(p) {}
RenderContext::RenderContext(Device *d)
    : RenderContextBase(&extensionData, &privateMembers), extensionData(this), privateMembers(this), device(d) {}
void RenderContext::BeginFrame() {
    RenderContextPrivate::gBeginEndNestLevel++;
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    Device::Get()->privateData.SetCurrentRenderContext(this);
    GeoPrimStateExtension::gCullClockwise = privateMembers.cullClockwise;
    GeoPrimStateExtension::gZWritesEnable = privateMembers.zWritesEnable;
    GeoPrimStateExtension::SetRenderContextStates();
    if (RenderContextExtensionBase::gEnableDestinationAlpha)
        GXSetDither(privateMembers.ditherEnable ? 1 : 0);
    ViewPortExtension::gpFirstViewPort = 0;
}
