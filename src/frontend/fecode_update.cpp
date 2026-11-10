// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FECodeListBox::Update(float dt) {
    if (selectCallback) selectCallback(this);
    float rate = blinkRate;
    float value = blink + rate * dt;
    blink = value;
    if (value < 0.0f) {
        blink = 0.0f;
        blinkRate = -rate;
    } else if (!(value <= 1.0f)) {
        blink = 1.0f;
        blinkRate = -rate;
    }
}
