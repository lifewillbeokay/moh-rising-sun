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

void MotionBlur::SetFrameBlur(float amount, float rampIn, float rampOut, float duration) {
    gAmount = amount;
    gRampIn = rampIn;
    gRampOut = rampOut;
    gDuration = duration;
    gCurrentAmount = 0.0f;
    gState = 1;
    gTransitionTimer = rampIn;
    if (rampIn == 0.0f)
        gDeltaAmount = amount;
    else
        gDeltaAmount = amount / rampIn;
    if (gDuration == 0.0f && gRampOut == 0.0f) {
        if (gAmount == gCurrentAmount) {
            gState = 0;
            gTransitionTimer = 0.0f;
            gDeltaAmount = 0.0f;
        } else {
            gDeltaAmount = (gAmount - gCurrentAmount) / gRampIn;
        }
    }
}

void MotionBlur::StopFrameBlur() {
    gState = 0;
    gCurrentAmount = 0.0f;
}

void MotionBlur::Update(float dt) {
    gPaused = 0;
    if (!(gCurrentAmount <= 0.95f))
        gCurrentAmount = 0.95f;
    if (!(gCurrentAmount >= 0.0f))
        gCurrentAmount = 0.0f;
    gPrevAmount = gCurrentAmount;
    EAGL::Device::Get()->GetCurrentRenderContext()->extension.EnableMotionBlur(gCurrentAmount, true);
    switch (gState) {
    case 0:
        break;
    case 1:
        gCurrentAmount = gDeltaAmount * dt + gCurrentAmount;
        if (!(gCurrentAmount <= gAmount)) {
            gCurrentAmount = gAmount;
            gState = 2;
            gTransitionTimer = gDuration;
            gDeltaAmount = 0.0f;
            if (gDuration == 0.0f && gRampOut == 0.0f)
                gState = 0;
        }
        break;
    case 2:
        gTransitionTimer -= dt;
        if (!(gTransitionTimer > 0.0f)) {
            gState = 3;
            gTransitionTimer = gRampOut;
            if (gRampOut == 0.0f)
                gDeltaAmount = -gAmount;
            else
                gDeltaAmount = -gAmount / gRampOut;
        }
        break;
    case 3:
        gCurrentAmount = gDeltaAmount * dt + gCurrentAmount;
        if (!(gCurrentAmount >= 0.0f))
            gState = 0;
        break;
    }
}
