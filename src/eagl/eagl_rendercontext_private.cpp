// AI-assisted reconstruction of EAGL render-context private state; see docs/Rendering.md.
#pragma implementation "EAGLRenderContextExtensionBase.h"
#include <dolphin/vi/vifuncs.h>
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
inline RenderContextPrivateBase::RenderContextPrivateBase(RenderContext *c)
    : unknown_0(0), context(c), currentViewPort(0), firstViewPort(0) {}
RenderContextPrivate::RenderContextPrivate(RenderContext *c)
    : RenderContextPrivateBase(c), renderMode(0), owner(c), width(640.0f), height(448.0f),
      depthBits(24), depthFormat(24), viX(-1), viY(-1), viXOrigin(-1), viYOrigin(-1), unknown_3c(0),
      externalFrameBuffer(0), unknown_88(0), fifoSize(0x40000), unknown_90(0), motionBlurTAR(0),
      unknown_98(0.0f), unknown_9c(0), unknown_a0(true), syncToVBL(true), cullClockwise(false),
      zWritesEnable(true), ditherEnable(true) {}
RenderContextPrivate::~RenderContextPrivate() {
    if (gVSyncCallbackRefCount) {
        gVSyncCallbackRefCount--;
        VISetPostRetraceCallback(0);
    }
    if (motionBlurTAR) {
        MEM_free((void *)motionBlurTAR->GetShape());
        delete motionBlurTAR;
    }
}
