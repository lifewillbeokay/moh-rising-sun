#ifndef GAME_EAGL_RENDER_CONTEXT_EXTENSION_BASE_H
#define GAME_EAGL_RENDER_CONTEXT_EXTENSION_BASE_H
// AI-assisted runtime view; see docs/Rendering.md. The class, method and static-member names come
// from the symbol table; instance member names are inferred from use. As with the private base, the
// destructor is inlined by the derived destructor and has one out-of-line copy.
#pragma interface
#include "EAGLGeoPrimState.h"
namespace EAGL {
enum MipMapModeOverride {MMMO_INHERIT=-1};
enum FilterModeOverride {FMO_INHERIT=-1};
enum AnisotropyOverride {AO_INHERIT=-1};
// Enumerator values are not established by the code that receives them.
enum FogTableMode {};
class RenderContext;
class RenderContextExtensionBase {
  public:
    bool fogEnable;       // +0
    float fogStart;       // +4
    float fogEnd;         // +8
    FogTableMode fogMode; // +12
    Colour fogColour;     // +16
    bool fogRangeAdjust;  // +20
    RenderContext *base;  // +24
    inline RenderContextExtensionBase(RenderContext *);
    virtual ~RenderContextExtensionBase() {}
    void GetFogEnable(bool &) const;
    void SetFogEnable(bool);
    void SetFogTableMode(FogTableMode);
    void SetFogStart(float);
    void SetFogEnd(float);
    void SetFogColour(Colour);
    void SetFogRangeAdjustEnable(bool);
    void EnactFogSettings() const;
    static bool GetAlphaWritesOverride(AlphaWritesOverride &);
    static bool GetMipMapModeOverride(MipMapModeOverride &);
    static bool GetFilterModeOverride(FilterModeOverride &);
    static bool GetMipmapLODBiasOverride(float &);
    static void GetMaxAnisotropyOverride(AnisotropyOverride &);
    static bool gEnableDestinationAlpha;
    static float gOverrideMipmapLODBias;
    static FilterModeOverride gOverrideFilterMode;
    static AlphaWritesOverride gOverrideAlphaWrites;
    static MipMapModeOverride gOverrideMipMapMode;
    static AnisotropyOverride gOverrideMaxAniso;
};
}
#endif
