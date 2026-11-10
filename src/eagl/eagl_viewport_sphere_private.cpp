// AI-assisted reconstruction of EAGL viewport private state; see docs/Rendering.md.
#include <dolphin/gx/GXPixel.h>
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
bool ViewPort::IsSphereInView(const COORD3 &center, float radius) {
    COORD3 p;
    Transform view;
    Transform *transform = &view;
    transform->BuildMatrix(&EAGLInternal::gGlobalViewMatrix);
    transform->TransformPoint(center, p);
    float d = p.z + privateData.nearClip;
    if (d > radius)
        return false;
    d = p.z + privateData.farClip;
    d = -d;
    if (d > radius)
        return false;
    if (privateData.projectionType == 0) {
        d = p.z * privateData.sidePlanes[1].tangent + p.x;
        if (d > 0.0f) {
            d *= privateData.sidePlanes[1].sine;
            if (d > radius)
                return false;
        } else {
            d = p.z * privateData.sidePlanes[0].tangent + p.x;
            if (d < 0.0f) {
                d *= -privateData.sidePlanes[0].sine;
                if (d > radius)
                    return false;
            }
        }
        d = p.z * privateData.sidePlanes[2].tangent + p.y;
        if (d > 0.0f) {
            d *= privateData.sidePlanes[2].sine;
            if (d > radius)
                return false;
        } else {
            d = p.z * privateData.sidePlanes[3].tangent + p.y;
            if (d < 0.0f) {
                d *= -privateData.sidePlanes[3].sine;
                if (d > radius)
                    return false;
            }
        }
    }
    return true;
}
ViewPortPrivate::ViewPortPrivate(ViewPort *viewPort) {
    context = 0;
    projectionType = 1;
    backgroundColour.packed = 0;
    active = 0;
    unknown_194 = 0;
    next = 0;
    owner = viewPort;
    x = -1;
    y = -1;
    width = -1;
    height = -1;
    shapeZ[0] = -1.0f;
    shapeZ[1] = -1.0f;
    fieldOfView = -1.0f;
    aspect = -1.0f;
    nearClip = -1.0f;
    farClip = -1.0f;
    projectionMatrix[0][0] = 1.0f;
    projectionMatrix[0][1] = 0.0f;
    projectionMatrix[0][2] = 0.0f;
    projectionMatrix[0][3] = 0.0f;
    projectionMatrix[1][0] = 0.0f;
    projectionMatrix[1][1] = 1.0f;
    projectionMatrix[1][2] = 0.0f;
    projectionMatrix[1][3] = 0.0f;
    projectionMatrix[2][0] = 0.0f;
    projectionMatrix[2][1] = 0.0f;
    projectionMatrix[2][2] = 1.0f;
    projectionMatrix[2][3] = 0.0f;
    projectionMatrix[3][0] = 0.0f;
    projectionMatrix[3][1] = 0.0f;
    projectionMatrix[3][2] = 0.0f;
    projectionMatrix[3][3] = 1.0f;
}
void ViewPortPrivate::EnactFogSettings() {
    if (!context)
        return;
    RenderContextExtension *extension = context->extension;
    GXColor colour = *(const GXColor *)&extension->fogColour;
    if (extension->fogEnable) {
        GXSetFog((GXFogType)context->extension->fogMode, context->extension->fogStart, context->extension->fogEnd,
                 nearClip, farClip, colour);
        if (context->extension->fogRangeAdjust) {
            GXFogAdjTable table;
            GXInitFogAdjTable(&table, (unsigned short)width, projectionMatrix);
            int center = x + width / 2;
            if (center < 0)
                center = 0;
            GXSetFogRangeAdj(1, (unsigned short)center, &table);
        } else {
            GXSetFogRangeAdj(0, 0, 0);
        }
    } else {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, colour);
    }
}
