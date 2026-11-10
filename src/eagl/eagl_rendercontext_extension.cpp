// AI-assisted reconstruction of EAGL render-context extension state; see docs/Rendering.md.
#include <dolphin/gx/GXFrameBuffer.h>
#include <dolphin/gx/GXTexture.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/vi/vifuncs.h>
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
inline RenderContextExtensionBase::RenderContextExtensionBase(RenderContext *c) {
    fogEnable = false;
    fogStart = 0.0f;
    fogEnd = 0.0f;
    fogMode = (FogTableMode)0;
    fogColour.packed = 0;
    fogRangeAdjust = true;
    base = c;
}
RenderContextExtension::RenderContextExtension(RenderContext *c)
    : RenderContextExtensionBase(c), context(c), unknown_24(0) {}
RenderContextExtension::~RenderContextExtension() {}
void RenderContextExtensionBase::SetFogEnable(bool enable) {
    fogEnable = enable;
    EnactFogSettings();
}
void RenderContextExtensionBase::GetFogEnable(bool &enable) const {
    enable = fogEnable;
}
void RenderContextExtensionBase::SetFogTableMode(FogTableMode mode) {
    fogMode = mode;
    EnactFogSettings();
}
void RenderContextExtensionBase::SetFogStart(float start) {
    fogStart = start;
    EnactFogSettings();
}
void RenderContextExtensionBase::SetFogEnd(float end) {
    fogEnd = end;
    EnactFogSettings();
}
void RenderContextExtensionBase::SetFogColour(Colour colour) {
    fogColour = colour;
    EnactFogSettings();
}
void RenderContextExtensionBase::SetFogRangeAdjustEnable(bool enable) {
    fogRangeAdjust = enable;
    EnactFogSettings();
}
void RenderContextExtensionBase::EnactFogSettings() const {
    ViewPort *viewPort = base->GetCurrentViewPort();
    if (viewPort)
        viewPort->privateData.EnactFogSettings();
}
bool RenderContextExtensionBase::GetMipmapLODBiasOverride(float &bias) {
    bias = gOverrideMipmapLODBias;
    return true;
}
bool RenderContextExtensionBase::GetFilterModeOverride(FilterModeOverride &mode) {
    mode = gOverrideFilterMode;
    return true;
}
bool RenderContextExtensionBase::GetAlphaWritesOverride(AlphaWritesOverride &mode) {
    mode = gOverrideAlphaWrites;
    return true;
}
bool RenderContextExtensionBase::GetMipMapModeOverride(MipMapModeOverride &mode) {
    mode = gOverrideMipMapMode;
    return true;
}
void RenderContextExtensionBase::GetMaxAnisotropyOverride(AnisotropyOverride &aniso) {
    aniso = gOverrideMaxAniso;
}
void RenderContextExtension::SetRenderMode(GXRenderModeObj &mode, unsigned short overScanY) {
    RenderContextPrivate::gRenderMode = &mode;
    RenderContextPrivate::gOverScanYAdjust = overScanY;
}
bool RenderContextExtension::GetDebugTokenEnable() {
    return gIsDebugTokenEnabled;
}
void RenderContextExtension::SetVISize(int x, int y) {
    context->privateMembers.viX = x;
    context->privateMembers.viY = y;
}
void RenderContextExtension::SetVideoOrigin(int x, int y) {
    if (x < -40)
        x = -40;
    else if (x > 40)
        x = 40;
    if (y < -16)
        y = -16;
    else if (y > 16)
        y = 16;
    if (context->privateMembers.renderMode) {
        context->privateMembers.renderMode->viXOrigin = x + 40;
        context->privateMembers.renderMode->viYOrigin = y + 16;
        VIConfigure(context->privateMembers.renderMode);
        VIFlush();
    } else {
        context->privateMembers.viXOrigin = (unsigned short)(x + 40);
        context->privateMembers.viYOrigin = (unsigned short)(y + 16);
    }
}
void RenderContextExtension::GetXFBInformation(void *&frameBuffer, GXRenderModeObj *&mode) const {
    frameBuffer = context->privateMembers.externalFrameBuffer;
    mode = context->privateMembers.renderMode;
}
void RenderContextExtension::SetScreenClearOptimization(bool enable) {
    gDoScreenClear = enable ? 1 : 0;
}
bool RenderContextExtension::GetScreenClearOptimization() {
    return gDoScreenClear == 1;
}
void RenderContextExtension::SetFifoSize(unsigned int size) {
    if (!RenderContextPrivate::gFrameBuffersInitialized)
        context->privateMembers.fifoSize = size;
}
void RenderContextExtension::SetEFBCopy(int left, int top, int width, int height, int dstWidth, int dstHeight,
                                        GXTexFmt format, unsigned char mipmap) {
    GXSetTexCopySrc(left, top, width, height);
    GXSetTexCopyDst(dstWidth, dstHeight, format, mipmap);
}
