// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp: the drawing state
// shared by all soldiers. Class and method names come from the symbol table; enum spellings follow
// EAGLGeoPrimState.h.
#include "SceneObjects.h"

namespace EAGL {
enum PrimitiveType { PRIMITIVE_TRIANGLE_STRIP = 0x98 };
enum Shading { S_FLAT, S_GOURAUD, S_SPECULAR };
enum AlphaBlendMode { ABM_OFF, ABM_BLEND };
enum TransparencyMethod { TM_OPAQUE, TM_ALPHA };
enum DepthTestMethod { DTM_UNKNOWN_1 = 1 }; // Only this value's use is established here.
class GeoPrimStateExtension {
  public:
    void SetZWritesEnable(bool);
};
class GeoPrimState {
  public:
    GeoPrimStateExtension extension;
    bool SetPrimitiveType(PrimitiveType);
    bool SetShading(Shading);
    bool SetTextureEnable(bool);
    bool SetAlphaBlendMode(AlphaBlendMode);
    bool SetTransparencyMethod(TransparencyMethod);
    bool SetDepthTestMethod(DepthTestMethod);
    bool SetAlphaCompareValue(int);
    bool SetCullEnable(bool);
};
class DrawTextured {
  public:
    void Init();
    GeoPrimState *GetState();
};
}
// The original file's private drawing object; the unit reaches it as an external.
extern EAGL::DrawTextured eagl_draw_tex;

void CSoldierObject::InitClass() {
    eagl_draw_tex.Init();
    EAGL::GeoPrimState *state = eagl_draw_tex.GetState();
    state->SetPrimitiveType(EAGL::PRIMITIVE_TRIANGLE_STRIP);
    state->SetShading(EAGL::S_GOURAUD);
    state->SetTextureEnable(true);
    state->SetAlphaBlendMode(EAGL::ABM_BLEND);
    state->SetTransparencyMethod(EAGL::TM_ALPHA);
    state->extension.SetZWritesEnable(true);
    state->SetDepthTestMethod(EAGL::DTM_UNKNOWN_1);
    state->SetAlphaCompareValue(0);
    state->SetCullEnable(false);
}
