// AI-assisted GR8E69 reconstruction; see docs/research/feng-lifecycle.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEMessageResponse.h"
#include "ShellManager.h"
extern unsigned long PadReleasedHash[];
void FEngine::ProcessMouseForPackage(FEPackage *package) {
    if (!(package->controlMask & 1) || !resetRequested) return;
    float x = (float)mouse.x, y = (float)mouse.y;
    for (unsigned long button = 0; button < 2 && resetRequested; ++button) {
        unsigned short mask = 1 << button;
        bool pressed = mouse.WasPressed(mask) != 0;
        bool released = mouse.WasReleased(mask) != 0;
        if (mouse.WasHeld(mask)) mouse.HeldFor(mask);
        FEObject *current = package->currentButton;
        if (pressed) {
            if (!button) {
                pressedObjects[4] = current;
                if (current && current->FindResponse(0xc407210) && game->DoesPointTouchObject(x, y, current) == 1) {
                    QueueMessage(0xc407210, 0, package, package->currentButton, 1);
                    QueueMessage(0xc407210, package->currentButton, package, (FEObject *)-5, 1);
                } else if (package->FindResponse(0x406415e3)) {
                    QueueMessage(0x406415e3, 0, package, (FEObject *)-3, 1);
                    QueueMessage(0x406415e3, 0, package, (FEObject *)-5, 1);
                }
            } else if (button == 1) {
                if (current && current->FindResponse(0x14dd0705) && game->DoesPointTouchObject(x, y, current) == 1) {
                    QueueMessage(0x14dd0705, 0, package, current, 1);
                    QueueMessage(0x14dd0705, current, package, (FEObject *)-5, 1);
                } else if (package->FindResponse(0x14dd0705)) {
                    QueueMessage(0x14dd0705, 0, package, (FEObject *)-3, 1);
                    QueueMessage(0x14dd0705, 0, package, (FEObject *)-5, 1);
                }
            }
        }
        if (released && !button) {
            if (pressedObjects[4] == current && current) {
                pressedObjects[4] = 0;
                if (current->FindResponse(0x936a6a7f)) {
                    QueueMessage(0x936a6a7f, 0, package, current, 1);
                    QueueMessage(0x936a6a7f, current, package, (FEObject *)-5, 1);
                }
            }
            if (package->FindResponse(0x936a6a7f)) {
                QueueMessage(0x936a6a7f, 0, package, (FEObject *)-3, 1);
                QueueMessage(0x936a6a7f, 0, package, (FEObject *)-5, 1);
            }
        }
        if (messageQueue.count) ProcessMessageQueue();
    }
    if (mouse.moved) {
        FEObject *current = package->currentButton;
        if (current && game->DoesPointTouchObject(x, y, current) != 1) {
            FEObject *next = 0;
            for (unsigned long i = 0; i < package->buttonMap.count; ++i) {
                next = package->buttonMap.buttons[i];
                if (!(next->flags & 0x4000000) && game->DoesPointTouchObject(x, y, next)) break;
                next = 0;
            }
            if (next) {
                for (unsigned long i = 4; i < 19; ++i) {
                    if (pressedObjects[i]) {
                        pressedObjects[i] = 0;
                        unsigned long message = i == 4 ? 0x936a6a7f : PadReleasedHash[i];
                        if (current && current->FindResponse(message)) {
                            QueueMessage(message, 0, package, current, 1);
                            QueueMessage(message, current, package, (FEObject *)-5, 1);
                        }
                    }
                }
                package->SetCurrentButton(this, next, true);
            }
        }
    }
}
#endif
