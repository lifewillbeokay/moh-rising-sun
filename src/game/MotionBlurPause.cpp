// The motion blur fade: a ramp up to an amount, a hold and a ramp down. Class, method and static names
// come from the symbol table; the state numbers' meanings are inferred from this code.
#include "SceneObjects.h"

namespace EAGL {
class RenderContextExtension {
public:
    void EnableMotionBlur(float, bool);
};
class RenderContext {
public:
    unsigned char unknown00[16];
    RenderContextExtension extension;
};
class Device {
public:
    static Device *Get();
    RenderContext *GetCurrentRenderContext() const;
};
}

void MotionBlur::Pause() {
    gPaused = 1;
    EAGL::RenderContext *context = EAGL::Device::Get()->GetCurrentRenderContext();
    if (context)
        context->extension.EnableMotionBlur(0.0f, true);
}
