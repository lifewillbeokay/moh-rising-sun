#ifndef GAME_EAGL_RENDER_CONTEXT_H
#define GAME_EAGL_RENDER_CONTEXT_H
// AI-assisted runtime views of EAGL's render context; see docs/Rendering.md. Class, method and
// static-member names come from the symbol table. Instance member names are inferred from use and
// only the offsets the reconstructed code touches are established.
#include "EAGLGeoPrimState.h"
#include "EAGLMemory.h"
#include "TARExtension.h"
#include "EAGLRenderContextPrivateBase.h"
#include "EAGLRenderContextExtensionBase.h"
#include "EAGLViewPort.h"
#include <dolphin/gx/GXStruct.h>
namespace EAGL {
// Enumerator values are not established by the code that receives them.
enum DepthBufferType {};
class Device;
}
namespace EAGLInternal {
class DevicePrivate {
  public:
    void SetCurrentRenderContext(EAGL::RenderContext *);
};

class RenderContextPrivate : public RenderContextPrivateBase {
  public:
    // Offsets below are relative to this object, which sits at +56 in EAGL::RenderContext.
    GXRenderModeObj *renderMode; // +20
    EAGL::RenderContext *owner;  // +24
    float width, height;         // +28, +32
    int depthBits, depthFormat;  // +36, +40: inferred, both default to 24.
    int viX, viY;                // +44, +48: default -1.
    int viXOrigin, viYOrigin;    // +52, +56: default -1.
    unsigned int unknown_3c;     // +60
    unsigned char unknown_40[0x40];
    void *externalFrameBuffer;   // +128
    int blackFrameCount;         // +132: inferred, counts down to VISetBlack(0).
    unsigned int unknown_88;     // +136
    unsigned int fifoSize;       // +140
    unsigned int unknown_90;     // +144
    EAGL::TAR *motionBlurTAR;    // +148
    float unknown_98;            // +152
    unsigned int unknown_9c;     // +156
    bool unknown_a0;             // +160
    bool syncToVBL;              // +164
    bool cullClockwise;          // +168
    bool zWritesEnable;          // +172
    bool ditherEnable;           // +176
    RenderContextPrivate(EAGL::RenderContext *);
    virtual ~RenderContextPrivate();
    struct GPBreak {
        unsigned int pointer;
        unsigned int token;
        void (*callback)(unsigned int);
        void ProcessGPBreak();
        bool ReleaseGPBreak(GPBreak &);
        void SetGPBreak(unsigned int, void (*)(unsigned int));
        static bool gBreakSet;
    };
    static int gBeginEndNestLevel;
    static int gEndFrameProcessed;
    static unsigned short gCurrentToken;
    static unsigned short gBreakToken;
    static int gBreakTokenCount;
    static unsigned short gLastToken;
    static int gDrawsDone;
    static bool gUseVSyncCallback;
    static int gVSyncCallbackRefCount;
    static unsigned int gFrameCycles;
    static unsigned int gLastCycleCount;
    static GXRenderModeObj *gRenderMode;
    static unsigned short gOverScanYAdjust;
    static int gFrameBuffersInitialized;
    static int gWaitingForEndFrameToken;
    static volatile int gGPBreakPointHead;
    static volatile int gGPBreakPointTail;
    static GPBreak gGPBreaklist[128];
    static void ReceiveToken(unsigned short);
    static void ProcessGPBreakPoint();
    static void SetGPBreakPoint(unsigned int, void (*)(unsigned int));
    static void ReleaseGPBreakPoint();
    static unsigned short gPixPassedZTest[4096];
    static unsigned short gPixSubmitted[4096];
    static unsigned char gVisibilityValid[4096];
};
}
extern "C" int MEM_free(void *);
namespace EAGL {
class RenderContextExtension : public RenderContextExtensionBase {
  public:
    RenderContext *context; // +32
    int unknown_24;         // +36
    RenderContextExtension(RenderContext *);
    virtual ~RenderContextExtension();
    static void WaitForDrawsToComplete();
    static void SetVolatileBufferSize(unsigned int);
    static void *GetCurrentVolatileBuffer();
    static void StartDataStuffing(void *, unsigned int);
    static void StuffData(const void *, unsigned int);
    static void StopDataStuffing();
    void SetRenderMode(GXRenderModeObj &, unsigned short);
    static void SendDebugToken();
    static bool GetDebugTokenEnable();
    void SetVISize(int, int);
    void SetVideoOrigin(int, int);
    void GetXFBInformation(void *&, GXRenderModeObj *&) const;
    static void SetScreenClearOptimization(bool);
    static bool GetScreenClearOptimization();
    void SetFifoSize(unsigned int);
    void SetEFBCopy(int, int, int, int, int, int, GXTexFmt, unsigned char);
    void GetEFBAsTAR(TAR *, bool, bool, bool);
    static unsigned char gDoScreenClear;
    static bool gIsDebugTokenEnabled;
    static void *gVolatileBuffers[2];
    static unsigned int gVolatileBufferSize;
    static int gCurrentVolatileBuffer;
    static bool gDataStuffing;
};
class RenderContextBase {
  public:
    RenderContextExtension *extension;              // +0
    unsigned int unknown_4;                         // +4
    EAGLInternal::RenderContextPrivateBase *privateData; // +8
    inline RenderContextBase(RenderContextExtension *, EAGLInternal::RenderContextPrivateBase *);
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void GetSize(float &, float &) const = 0;
    virtual ~RenderContextBase();
    ViewPort *NewViewPort();
    void DeleteViewPort(ViewPort *);
    ViewPort *GetCurrentViewPort() const;
};
class RenderContext : public RenderContextBase {
  public:
    RenderContextExtension extensionData;       // +16
    EAGLInternal::RenderContextPrivate privateMembers; // +56
    Device *device;                             // +236
    RenderContext(Device *);
    virtual ~RenderContext();
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void GetSize(float &, float &) const;
    void SetSize(float, float);
    void SetZBufferDepth(int);
    bool SetDepthBufferType(DepthBufferType);
    void SetSyncToVBL(bool);
    bool SetZWritesEnable(bool);
    static int gCurrentFrameNumber;
};
}
#endif
