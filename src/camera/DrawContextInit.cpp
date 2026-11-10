// A fragment of the original draw_context.cpp: the queued draw commands by render bin, and
// the EAGL draw context. Class, method and static names come from the symbol table; member names and the
// command queue's local name are inferred from this code. The queue has the shape of STLport's
// priority_queue, shown by the pop_heap and push_heap instances, which the project's headers lack.
#include "profile.h"
#include <vector>
#include <algorithm>
#include "AlgorithmTypes.h"
#include "Camera.h"
#include "Screen.h"
#include "CVector3.h"

extern "C" int ScreenshotIsActive();
extern "C" void GetScreenshotViewportOffsets(float *, float *);
extern "C" void GXGetViewportv(float *);
extern "C" void GXSetViewport(float, float, float, float, float, float);

struct MATRIX4;
struct COORD3;

namespace EAGL {
enum ClearFlags { ClearFlagsUnknown = 0 };
enum PrimitiveType { PrimitiveTypeUnknown = -4 };
enum Shading { S_FLAT, S_GOURAUD, S_SPECULAR };
enum TransparencyMethod { TransparencyMethodNone };
enum AlphaBlendMode { AlphaBlendModeNone };
enum DepthTestMethod { DepthTestMethodUnknown = 6 };

class ViewPort {
public:
    void SetViewMatrix(const MATRIX4 &);
    void SetShape(float, float, float, float, float, float);
    void SetPerspective(float, float, float, float);
    void SetOrthographicScreenSpace(float, float);
    void BeginView();
    void EndView();
    void ClearViewPort(ClearFlags);
    bool IsSphereInView(const COORD3 &, float);
    static MATRIX4 *gpModelViewProjectionMatrix;
};

class RenderContext {
public:
    bool SetZWritesEnable(bool);
};

class GeoPrimState {
public:
    GeoPrimState();
    ~GeoPrimState();
    void *operator new(unsigned int, void *p) { return p; }
    bool SetPrimitiveType(PrimitiveType);
    bool SetShading(Shading);
    bool SetTextureEnable(bool);
    bool SetAlphaTestEnable(bool);
    bool SetTransparencyMethod(TransparencyMethod);
    bool SetAlphaBlendMode(AlphaBlendMode);
    bool SetDepthTestMethod(DepthTestMethod);
    unsigned char storage[60];
};

// The fields' order comes from ShutdownClass's stores.
class GeoPrim {
public:
    void *renderMethod;
    int field04;
    unsigned char unknown08[8];
    int field10;
    void *field14;
};

class DrawArray {
public:
    DrawArray();
    ~DrawArray();
    void *operator new(unsigned int, void *p) { return p; }
    void SetGeoPrim(GeoPrim *);
    bool SetPrimitiveType(PrimitiveType);
    bool SetParamName(const char *);
    bool SetVar(const char *, void *, int, int);
    unsigned char storage[60];
};

class DynamicLoader {
public:
    static void *GetRegisteredVar(const char *, bool &);
};
}

extern char rm_SingleDrawGouraud[];

// The render context at +0x74 of the screen; the rest of the object is not needed here.
struct CGlobalScreen {
    unsigned char unknown00[0x74];
    EAGL::RenderContext *renderContext;
};
extern CGlobalScreen g_screen;

class CFog;

class CViewport {
public:
    float left, top, right, bottom;
    unsigned char unknown10[0x64 - 0x10];
    int dirty;
};

class DrawCommandQueue {
public:
    bool empty() const { return c.empty(); }
    unsigned int size() const { return c.size(); }
    void push(const DrawCommandData &command) {
        c.push_back(command);
        _STL::push_heap(c.begin(), c.end(), comp);
    }
    void pop() {
        _STL::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
    const DrawCommandData &top() const { return c.front(); }

    _STL::vector<DrawCommandData> c;
    _STL::less<DrawCommandData> comp;
};

enum ERenderBins { RenderBinUnknown = 0 };

class CDrawContext {
public:
    CDrawContext(const CCamera &, const CViewport &, const CScreen &);
    void InitCommands();
    void AddCommand(ERenderBins bin, const DrawCommand &command);
    void ExecCommands();

    unsigned char unknown000[192];
    CMatrix *modelView;
    unsigned char unknown0c4[212 - 196];
    CCamera *camera;
    CFog *fog;
    unsigned char unknown0dc[276 - 220];
    CViewport *viewport;
    CViewport *appliedViewport;
    CScreen *screen;
    DrawCommandQueue bins[13];
    virtual ~CDrawContext();
    virtual void BeginDraw(bool) = 0;
    virtual void EndDraw() = 0;
    virtual void ClearZBuffer() = 0;
    virtual bool IsSphereInView(const CVector3 &, float) = 0;

    static int b_ReverseParticleVsTranslucentSorting;
};

class CDrawContextEAGL : public CDrawContext {
public:
    CDrawContextEAGL(const CCamera &, const CViewport &, const CScreen &, EAGL::ViewPort *, float);
    virtual ~CDrawContextEAGL();
    virtual void BeginDraw(bool);
    virtual void EndDraw();
    virtual void ClearZBuffer();
    virtual bool IsSphereInView(const CVector3 &, float);
    static void InitClass();
    static void ShutdownClass();

    unsigned char unknown228[8];
    float unknown230;
    EAGL::ViewPort *viewPort;
};

// File statics of the original, whose storage is not reconstructed.
extern EAGL::GeoPrimState sGeoPrimState;
extern EAGL::DrawArray sDrawArray;
extern EAGL::GeoPrim sGeoPrim;
extern char sVertices[], sVertColors[], sParamBlock[];
extern int initialized;

void CDrawContextEAGL::InitClass() {
    if (!initialized) {
        sGeoPrimState.SetPrimitiveType((EAGL::PrimitiveType)-4);
        sGeoPrimState.SetShading(EAGL::S_GOURAUD);
        sGeoPrimState.SetTextureEnable(false);
        sGeoPrimState.SetAlphaTestEnable(false);
        sGeoPrimState.SetTransparencyMethod((EAGL::TransparencyMethod)0);
        sGeoPrimState.SetAlphaBlendMode((EAGL::AlphaBlendMode)0);
        sGeoPrimState.SetDepthTestMethod((EAGL::DepthTestMethod)6);
        sDrawArray.SetGeoPrim(&sGeoPrim);
        sDrawArray.SetPrimitiveType((EAGL::PrimitiveType)-4);
        sDrawArray.SetParamName("Parameters");
        sDrawArray.SetVar("Coordinates", sVertices, 16, -1);
        sDrawArray.SetVar("Colours", sVertColors, 4, -1);
        sDrawArray.SetVar("Parameters", sParamBlock, 0, -1);
        sDrawArray.SetVar("MVMatrix", EAGL::ViewPort::gpModelViewProjectionMatrix, 0, -1);
        initialized = 1;
    }
}

