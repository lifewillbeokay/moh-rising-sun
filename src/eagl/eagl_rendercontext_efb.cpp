// AI-assisted reconstruction of EAGL's embedded frame buffer copy; see docs/Rendering.md.
#include <dolphin/gx/GXFrameBuffer.h>
#include <dolphin/gx/GXManage.h>
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
void RenderContextExtension::GetEFBAsTAR(TAR *tar, bool clear, bool sync, bool filter) {
    GXRenderModeObj *mode = context->privateMembers.renderMode;
    if (filter) {
        unsigned char softFilter[7] = {9, 9, 9, 10, 9, 9, 9};
        GXSetCopyFilter(mode->aa, mode->sample_pattern, GX_TRUE, softFilter);
    } else {
        GXSetCopyFilter(mode->aa, mode->sample_pattern, GX_FALSE, mode->vfilter);
    }
    GXCopyTex((void *)ShapePixels(tar->GetShape()), clear);
    GXSetCopyFilter(mode->aa, mode->sample_pattern, GX_TRUE, mode->vfilter);
    if (sync)
        GXPixModeSync();
}
