// Scoped lighting storage used by the mesh block-4 comparisons.
// Offsets follow target accesses; this is not a complete historical light class.
// See docs/Rendering.md. No rendering implementation is imported.
#ifndef GAME_EAGL_MATERIAL_LIGHTING_H
#define GAME_EAGL_MATERIAL_LIGHTING_H
#include "EAGLMaterial.h"
namespace EAGLInternal {
struct MaterialLightColour {
    float rgb[3];
    unsigned char unknown_0c[4]; // Preserves the observed stride; fourth word unused.
};
struct MaterialLightData {
    COORD4 directions[3];           // +0x00, 16-byte stride; passed to TransformPoint.
    MaterialLightColour colours[3]; // +0x30, 16-byte stride.
    float ambient[3];               // +0x60. Accessed prefix ends at +0x6c; allocation unknown.
};
} // namespace EAGLInternal
#endif
