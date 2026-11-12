// FEJoyPad: per-button state. Bit n of a mask is button n; holdTime[n] counts how long it has been down.
#include "FEJoyPad.h"

FEJoyPad::FEJoyPad() { Reset(); }
void FEJoyPad::Reset() {
    current = 0;
    previous = 0;
    for (int i = 0; i < 32; i++) holdTime[i] = 0;
}
void FEJoyPad::Update(unsigned long buttons, unsigned long elapsed) {
    previous = current;
    current = buttons;
    for (int i = 0; i < 32; i++) {
        unsigned long mask = 1 << i;
        if (current & mask) {
            if (previous & mask) holdTime[i] += elapsed;
            else holdTime[i] = 0;
        }
    }
}
int FEJoyPad::WasPressed(unsigned long mask) {
    int result = 0;
    unsigned long down = current & mask;
    if (down == mask && (previous & down) != down) result = 1;
    return result;
}
int FEJoyPad::WasHeld(unsigned long mask) {
    bool result = false;
    unsigned long down = current & mask;
    if (down == mask) {
        unsigned long both = previous & down;
        result = both == down;
    }
    return result;
}
unsigned long FEJoyPad::HeldFor(unsigned long mask) {
    unsigned long shortest = 0xffffffff;
    for (int i = 0; i < 32; i++) {
        if (mask & (1 << i)) {
            unsigned long held = holdTime[i];
            if (held > shortest) held = shortest;
            shortest = held;
        }
    }
    return shortest;
}
int FEJoyPad::WasReleased(unsigned long mask) {
    bool result = false;
    if ((current & mask) != mask) {
        unsigned long both = previous & mask;
        result = both == mask;
    }
    return result;
}
void FEJoyPad::DecrementHold(unsigned long mask, unsigned long amount) {
    for (int i = 0; i < 32; i++) {
        if (mask & (1 << i)) {
            if (holdTime[i] > amount) holdTime[i] -= amount;
            else holdTime[i] = 0;
        }
    }
}
