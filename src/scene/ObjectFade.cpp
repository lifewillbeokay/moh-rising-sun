// A fragment of the original object fade code: a timed fade out or in of an object. Names as in
// SceneObjects.h, inferred from what these methods do.
#include "SceneObjects.h"

CObjectFadeManager::CObjectFadeManager() {
    state = 0;
}

void CObjectFadeManager::StartFadeOut(float duration, float delayFraction) {
    state = 2;
    time = 0.0f;
    this->duration = duration;
    delay = delayFraction;
}

void CObjectFadeManager::StartFadeIn(float duration, float delayFraction) {
    state = 1;
    time = 0.0f;
    this->duration = duration;
    delay = delayFraction;
}

int CObjectFadeManager::AreFading() {
    int fading = 0;
    if (state)
        fading = time > delay * duration;
    return fading;
}

int CObjectFadeManager::HalfDone() {
    return time > ((1.0f - delay) * 0.5f + delay) * duration;
}

void CObjectFadeManager::Update(float dt) {
    if (state) {
        time += dt;
        if (!(time <= duration) && state == 1)
            state = 0;
    }
}

float CObjectFadeManager::GetShadowAlphaScale() {
    float start = delay * duration;
    float value = (time - start) / start;
    if (!(value < 1.0f))
        value = 1.0f;
    return 1.0f - value;
}

float CObjectFadeManager::FadeValue() const {
    if (!state)
        return 1.0f;
    float value = 0.0f;
    float start = delay * duration;
    if (!(time <= start)) {
        value = (time - start) / ((1.0f - delay) * duration);
        if (!(value < 1.0f))
            value = 1.0f;
    }
    if (state == 1)
        return value;
    return 1.0f - value;
}
