// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEJoyPad.h"
#include "FEMessageResponse.h"
#include <string.h>
extern unsigned long PadButtonHash[], PadReleasedHash[], PadButtonHeldHash[], FEDirection_Message[];
// Four-byte records observed in the private ImpulseDir table.
struct FEImpulseDirection { unsigned char first, second; unsigned short direction; };
extern FEImpulseDirection ImpulseDir[];
void FEngine::ProcessPadsForPackage(FEPackage *package) {
    unsigned long active = package->controlMask & activeMask;
    if (!active) return;
    bool any = false;
    for (unsigned char pad = 0; pad < (unsigned long)joyPadCount; ++pad)
        if (active & (1 << pad)) any = any || joyPads[pad].previous || joyPads[pad].current;
    if (!any) return;
    unsigned long held[19];
    unsigned char heldBy[19], pressedBy[19], releasedBy[19];
    memset(held, 0, sizeof(held)); memset(heldBy, 0, sizeof(heldBy));
    memset(pressedBy, 0, sizeof(pressedBy)); memset(releasedBy, 0, sizeof(releasedBy));
    for (unsigned long button = 4; button < 19 && resetRequested; ++button) {
        unsigned long bit = 1 << button, pressed = 0, released = 0, holding = 0;
        for (unsigned char pad = 0; pad < (unsigned long)joyPadCount; ++pad) {
            unsigned long port = 1 << pad;
            if (active & port) {
                if (joyPads[pad].WasPressed(bit)) { pressed |= bit; pressedBy[button] |= port; }
                if (joyPads[pad].WasReleased(bit)) { released |= bit; releasedBy[button] |= port; }
                if (joyPads[pad].WasHeld(bit)) {
                    holding |= bit;
                    if (held[button] <= joyPads[pad].HeldFor(bit)) held[button] = joyPads[pad].HeldFor(bit);
                    heldBy[button] |= port;
                }
            }
        }
        if (button == 4 && package->unknown_20) {
            for (unsigned char pad = 0; pad < (unsigned long)joyPadCount; ++pad) {
                unsigned long port = 1 << pad;
                if (active & port) {
                    if (joyPads[pad].WasPressed(0x40)) { pressed |= 0x10; pressedBy[4] |= port; }
                    if (joyPads[pad].WasReleased(0x40)) { released |= 0x10; releasedBy[4] |= port; }
                    if (joyPads[pad].WasHeld(0x40)) {
                        holding |= 0x10;
                        if (held[4] <= joyPads[pad].HeldFor(0x40)) held[4] = joyPads[pad].HeldFor(0x40);
                        heldBy[4] |= port;
                    }
                }
            }
        }
        if (!(pressed || released || holding)) continue;
        FEObject *current = package->currentButton;
        if ((button == 7 || button == 8) && (holding & bit)) {
            unsigned long message = PadButtonHeldHash[button - 7];
            unsigned char ports = pressedBy[button];
            if (current && current->FindResponse(message)) {
                QueueMessage(message, 0, package, current, ports);
                QueueMessage(message, current, package, (FEObject *)-5, ports);
            } else if (package->FindResponse(message)) {
                QueueMessage(message, 0, package, (FEObject *)-3, ports);
                QueueMessage(message, 0, package, (FEObject *)-5, ports);
            }
        }
        if (pressed & bit) {
            pressedObjects[button] = current;
            if (button == 4) {
                if (current && current->FindResponse(0xc407210)) {
                    QueueMessage(0xc407210, 0, package, package->currentButton, pressedBy[4]);
                    QueueMessage(0xc407210, package->currentButton, package, (FEObject *)-5, pressedBy[4]);
                } else if (package->FindResponse(0x406415e3)) {
                    QueueMessage(0x406415e3, 0, package, (FEObject *)-3, pressedBy[4]);
                    QueueMessage(0x406415e3, 0, package, (FEObject *)-5, pressedBy[4]);
                }
                lastAcceptPad = pressedBy[4] >> 1;
                if (lastAcceptPad == 4) lastAcceptPad = 3;
            } else {
                unsigned long message = PadButtonHash[button];
                if (current && current->FindResponse(message)) {
                    QueueMessage(message, 0, package, current, pressedBy[button]);
                    QueueMessage(message, current, package, (FEObject *)-5, pressedBy[button]);
                } else if (package->FindResponse(message)) {
                    QueueMessage(message, 0, package, (FEObject *)-3, pressedBy[button]);
                    QueueMessage(message, 0, package, (FEObject *)-5, pressedBy[button]);
                }
            }
        }
        if (released & bit) {
            unsigned long message = PadReleasedHash[button];
            if (pressedObjects[button] == current && current) {
                pressedObjects[button] = 0;
                if (button == 4) message = 0x936a6a7f;
                if (current->FindResponse(message)) {
                    QueueMessage(message, 0, package, current, releasedBy[button]);
                    QueueMessage(message, current, package, (FEObject *)-5, releasedBy[button]);
                }
            }
            if (package->FindResponse(message)) {
                QueueMessage(message, 0, package, (FEObject *)-3, releasedBy[button]);
                QueueMessage(message, 0, package, (FEObject *)-5, releasedBy[button]);
            }
        }
        if (messageQueue.count) ProcessMessageQueue();
    }
    unsigned long pressed = 0;
    for (unsigned long direction = 0; direction < 4 && resetRequested; ++direction) {
        unsigned long bit = 1 << direction;
        for (unsigned char pad = 0; pad < (unsigned long)joyPadCount; ++pad) {
            unsigned long port = 1 << pad;
            if (active & port) {
                if (joyPads[pad].WasPressed(bit)) { pressed |= bit; pressedBy[direction] |= port; }
                joyPads[pad].WasReleased(bit);
                if (joyPads[pad].WasHeld(bit)) {
                    if (held[direction] <= joyPads[pad].HeldFor(bit)) held[direction] = joyPads[pad].HeldFor(bit);
                    heldBy[direction] |= port;
                }
            }
        }
    }
    for (unsigned long i = 0; i < 8 && resetRequested; ++i) {
        FEObject *current = package->currentButton;
        const FEImpulseDirection &impulse = ImpulseDir[i];
        unsigned long duration, down;
        unsigned char ports;
        if (impulse.second == 255) {
            down = (pressed >> impulse.first) & 1;
            duration = held[impulse.first]; ports = pressedBy[impulse.first] | heldBy[impulse.first];
        } else {
            duration = held[impulse.first] < held[impulse.second] ? held[impulse.first] : held[impulse.second];
            down = ((pressed >> impulse.first) & (pressed >> impulse.second)) & 1;
            ports = (pressedBy[impulse.first] & pressedBy[impulse.second]) | (heldBy[impulse.first] & heldBy[impulse.second]);
        }
        unsigned long bit = 1 << i, delay = (unknown_40 & bit) ? 120 : 320;
        if (duration < delay && !(down && !duration)) {
            if (!down && !duration) unknown_44 &= ~bit;
            if (messageQueue.count) ProcessMessageQueue();
            continue;
        }
        if (duration) unknown_44 |= bit;
        repeatAmounts[impulse.first] = delay;
        consumedDirections |= 1 << impulse.first;
        if (impulse.second != 255) {
            repeatAmounts[impulse.second] = delay;
            consumedDirections |= 1 << impulse.second;
        }
        unsigned long message = FEDirection_Message[impulse.direction];
        if (!current) {
            if (package->FindResponse(message)) {
                QueueMessage(message, 0, package, (FEObject *)-3, ports);
                QueueMessage(message, 0, package, (FEObject *)-5, ports);
            }
        } else {
            FEObject *next = 0;
            FEMessageResponse *response = current->FindResponse(message);
            if (response) {
                QueueMessage(message, 0, package, current, ports);
                if (!(current->flags & 0x80000) && response->FindResponse(0x104) == -1)
                    next = package->buttonMap.GetButtonFrom(current, impulse.direction, game, (FEButtonWrapMode)unknown_e4);
            } else {
                if (!(current->flags & 0x80000)) next = package->buttonMap.GetButtonFrom(current, impulse.direction, game, (FEButtonWrapMode)unknown_e4);
                QueueMessage(message, 0, package, (FEObject *)-3, ports);
            }
            QueueMessage(message, current, package, (FEObject *)-5, ports);
            if (next) {
                for (unsigned long button = 4; button < 19; ++button) {
                    if (pressedObjects[button]) {
                        pressedObjects[button] = 0;
                        unsigned long release = button == 4 ? 0x936a6a7f : PadReleasedHash[button];
                        if (current->FindResponse(release)) {
                            QueueMessage(release, 0, package, current, releasedBy[button]);
                            QueueMessage(release, current, package, (FEObject *)-5, releasedBy[button]);
                        }
                    }
                }
                package->SetCurrentButton(this, next, true);
            }
        }
        return;
    }
}
#endif
