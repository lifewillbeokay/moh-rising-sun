// AI-assisted reconstruction of EAGL render-context accessors; see docs/Rendering.md.
#pragma implementation "EAGLRenderContextPrivateBase.h"
#include <dolphin/gx/GXFifo.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os/OSCache.h>
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
// GX write-gather pipe, seen through its doubleword and byte ports.
volatile double gatherPipeDouble __attribute__((address(0xcc008000)));
volatile unsigned char gatherPipeByte __attribute__((address(0xcc008000)));
void RenderContext::SetSize(float w, float h) {
    privateMembers.width = w;
    privateMembers.height = h;
}
void RenderContext::GetSize(float &w, float &h) const {
    w = privateMembers.width;
    h = privateMembers.height;
}
void RenderContext::SetZBufferDepth(int) {}
bool RenderContext::SetDepthBufferType(DepthBufferType) {
    return false;
}
void RenderContext::SetSyncToVBL(bool enable) {
    privateMembers.syncToVBL = enable;
    RenderContextPrivate::gUseVSyncCallback = enable;
}
bool RenderContext::SetZWritesEnable(bool enable) {
    privateMembers.zWritesEnable = enable;
    GeoPrimStateExtension::gZWritesEnable = enable;
    GeoPrimStateExtension::SetRenderContextStates();
    return true;
}
void RenderContextExtension::WaitForDrawsToComplete() {
    GXDrawDone();
    RenderContext::gCurrentFrameNumber += 5;
}
void RenderContextExtension::SetVolatileBufferSize(unsigned int size) {
    if (!gVolatileBuffers[0])
        gVolatileBufferSize = size;
}
void *RenderContextExtension::GetCurrentVolatileBuffer() {
    return gVolatileBuffers[gCurrentVolatileBuffer];
}
void RenderContextExtension::StartDataStuffing(void *buffer, unsigned int size) {
    DCInvalidateRange(buffer, size);
    if (gDataStuffing)
        StopDataStuffing();
    gDataStuffing = true;
    GXRedirectWriteGatherPipe(buffer);
}
// Copies the buffer into the write-gather pipe, in 32-byte blocks, then doubles, then bytes.
void RenderContextExtension::StuffData(const void *data, unsigned int size) {
    const double *src = (const double *)data;
    while (size >= 32) {
        double a = src[0], b = src[1], c = src[2], d = src[3];
        size -= 32;
        src += 4;
        gatherPipeDouble = a;
        gatherPipeDouble = b;
        gatherPipeDouble = c;
        gatherPipeDouble = d;
    }
    while (size >= 8) {
        double a = *src;
        size -= 8;
        src++;
        gatherPipeDouble = a;
    }
    if (size) {
        const unsigned char *bytes = (const unsigned char *)src;
        do {
            gatherPipeByte = *bytes;
            bytes++;
        } while (--size);
    }
}
void RenderContextExtension::StopDataStuffing() {
    GXRestoreWriteGatherPipe();
    gDataStuffing = false;
}
