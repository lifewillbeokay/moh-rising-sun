// AI-assisted reconstruction of EAGL viewport accessors; see docs/Rendering.md.
#include "EAGLRenderContext.h"
using namespace EAGL;
using namespace EAGLInternal;
void ViewPort::GetShape(float &outX, float &outY, float &outWidth, float &outHeight, float &outNear, float &outFar) const {
    outX = privateData.x;
    outY = privateData.y;
    outWidth = privateData.width;
    outHeight = privateData.height;
    outNear = privateData.shapeZ[0];
    outFar = privateData.shapeZ[1];
}
