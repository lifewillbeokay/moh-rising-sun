#ifndef GAME_EAGL_VIEW_PORT_H
#define GAME_EAGL_VIEW_PORT_H
// AI-assisted runtime view of EAGL's viewport; see docs/Rendering.md. Class, method and static names
// come from the symbol table; instance member names are inferred from use. NewViewPort allocates 428
// bytes and the destructor frees 428, so that extent is established by more than one function.
// EndView has an out-of-line copy and is also inlined by ReBegin, so it is an inline body here.
#pragma interface
#include "EAGLGeoPrimState.h"
#include "EAGLTransform.h"
#include "EAGLMemory.h"
namespace EAGL {
class RenderContextBase;
class ViewPort;
class ViewPortExtension {
  public:
    ViewPort *owner; // +0
    ViewPortExtension(ViewPort *);
    ~ViewPortExtension();
    static ViewPort *gpFirstViewPort;
};
}
namespace EAGLInternal {
extern MATRIX4 gGlobalViewMatrix;
// Offsets are relative to this object, which sits at +12 in EAGL::ViewPort.
class ViewPortPrivate {
  public:
    EAGL::RenderContextBase *context; // +0
    int projectionType;               // +4
    EAGL::Colour backgroundColour;    // +8
    MATRIX4 modelMatrix;              // +12
    MATRIX4 viewMatrix;               // +76
    MATRIX4 viewProjectionMatrix;     // +140
    int x, y, width, height;          // +204
    float shapeZ[2];                  // +220: GetShape's fifth and sixth outputs
    float fieldOfView, aspect;        // +228
    float nearClip, farClip;          // +236 (the fog near and far inputs)
    unsigned char unknown_f4[0x18];   // +244
    struct Plane { float tangent, sine; } sidePlanes[4]; // +268: tan(a) and sin(90 - a) per side
    float sideScale[4];               // +300: left, right, top, bottom scale inputs
    unsigned char unknown_13c[0x10];  // +316
    float projectionMatrix[4][4];     // +332
    int active;                       // +396: inferred, tested by SetViewMatrix.
    unsigned int unknown_190;         // +400
    unsigned int unknown_194;         // +404
    EAGL::ViewPort *next;             // +408
    EAGL::ViewPort *owner;            // +412
    ViewPortPrivate(EAGL::ViewPort *);
    void EnactFogSettings();
    void ReBegin();
    static unsigned int gScreenColour;
};
}
namespace EAGL {
class ViewPort {
  public:
    ViewPortExtension extension;           // +0
    ViewPort *previous;                    // +4: inferred; values 0 and 1 mean no previous view.
    int modelSphereCull;                   // +8
    EAGLInternal::ViewPortPrivate privateData; // +12
    ViewPort(RenderContextBase *);
    ~ViewPort();
    // No out-of-line copies exist, so the bodies are in the files that use them.
    inline static void *operator new(unsigned int);
    inline static void operator delete(void *, unsigned int);
    void BeginView();
    inline void EndView();
    void GetShape(float &, float &, float &, float &, float &, float &) const;
    void SetPerspective(float, float, float, float);
    void SetViewMatrix(const MATRIX4 &);
    void SetOrthographicScreenSpace(float, float);
    void SetGuardBandScale(float);
    void SetEnableModelSphereCull(int);
    int GetEnableModelSphereCull();
    int GetProjectionType() const;
    void SetBackgroundColour(Colour);
    bool IsSphereInView(const COORD3 &, float);
    static MATRIX4 *gpProjectionMatrix;
    static MATRIX4 *gpViewProjectionMatrix;
    static MATRIX4 *gpModelViewProjectionMatrix;
};
}
#endif
