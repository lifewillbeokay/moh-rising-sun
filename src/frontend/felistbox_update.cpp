// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEListBox.h"
#include "FEngInterpolation.h"
#include <math.h>

void FEListBox::Update(float dt) {
    blink += blinkRate * dt;
    if (blink < 0.0f) {
        blink = 0.0f;
        blinkRate = -blinkRate;
    } else if (!(blink <= 1.0f)) {
        blink = 1.0f;
        blinkRate = -blinkRate;
    }
    if (listFlags & 2) {
        FEVector2 direction;
        direction = *(FEVector2 *)&unknown_98;
        float speed = (float)fabs(direction.x * unknown_7c + direction.y * unknown_80) * dt;
        FEVector2 step;
        step = direction;
        step.x *= speed;
        step.y *= speed;
        FEVector2 movement;
        movement = step;
        scrollX += movement.x;
        scrollY += movement.y;
        if (!(direction.x * targetX + direction.y * targetY -
              (direction.x * scrollX + direction.y * scrollY) >= 0.0f)) CompleteScroll();
    }
}

#endif
