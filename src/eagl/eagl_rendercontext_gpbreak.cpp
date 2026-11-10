// AI-assisted reconstruction of EAGL's GP tokens and break points; see docs/Rendering.md.
#include <dolphin/gx/GXManage.h>
#include <dolphin/gx/GXFifo.h>
extern "C" int OSDisableInterrupts();
extern "C" int OSRestoreInterrupts(int);
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
void RenderContextPrivate::ProcessGPBreakPoint() {
    gGPBreaklist[gGPBreakPointTail].ProcessGPBreak();
}
void RenderContextPrivate::SetGPBreakPoint(unsigned int token, void (*callback)(unsigned int)) {
    gGPBreaklist[gGPBreakPointHead].SetGPBreak(token, callback);
    gGPBreakPointHead++;
    if (gGPBreakPointHead > 127)
        gGPBreakPointHead = 0;
}
void RenderContextPrivate::ReleaseGPBreakPoint() {
    int next = gGPBreakPointTail + 1;
    if (next > 127)
        next = 0;
    if (gGPBreaklist[gGPBreakPointTail].ReleaseGPBreak(gGPBreaklist[next]))
        gGPBreakPointTail = next;
}
void RenderContextPrivate::GPBreak::ProcessGPBreak() {
    callback(token);
}
bool RenderContextPrivate::GPBreak::ReleaseGPBreak(GPBreak &next) {
    if (!pointer)
        return false;
    void *nextPointer = (void *)next.pointer;
    if (nextPointer) {
        GXEnableBreakPt(nextPointer);
        gBreakSet = true;
    } else {
        GXDisableBreakPt();
        gBreakSet = (bool)nextPointer;
    }
    pointer = 0;
    return true;
}
void RenderContextPrivate::GPBreak::SetGPBreak(unsigned int t, void (*c)(unsigned int)) {
    void *readPointer, *writePointer;
    GXFlush();
    GXGetFifoPtrs(GXGetCPUFifo(), &readPointer, &writePointer);
    if (pointer) {
        while (pointer) {}
    }
    token = t;
    callback = c;
    int level = OSDisableInterrupts();
    pointer = (unsigned int)writePointer;
    if (!gBreakSet) {
        gBreakSet = true;
        GXEnableBreakPt(writePointer);
    }
    OSRestoreInterrupts(level);
}
