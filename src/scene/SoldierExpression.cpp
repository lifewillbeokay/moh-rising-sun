// AI-assisted reconstruction from GR8E69. Part of the original Soldier_object.cpp; there is no
// Frontline counterpart, so parameter and state names are descriptive.
#include "SceneObjects.h"

void CSoldierObject::SetExpression(EExpressionType type, float time) {
    if (expression.pending >= 0) {
        expression.currentTime = time;
        expression.currentRate = 0.0f;
        expression.currentBlend = time;
        expression.pendingTime = 0.0f;
        expression.pendingRate = 0.0f;
        expression.current = type;
        expression.pendingBlend = 0.0f;
        expression.pending = -1;
    } else if (expression.current == -1) {
        expression.currentTime = time;
        expression.currentBlend = 0.0f;
        expression.current = type;
        expression.currentRate = time * 0.25f;
    } else if (expression.current != type) {
        expression.pendingTime = time;
        expression.currentRate = -expression.currentBlend * 0.25f;
        expression.pending = type;
        expression.pendingBlend = 0.0f;
        expression.pendingRate = time * 0.25f;
    }
}

void CSoldierObject::SetBlink(int state) {
    switch (state) {
    case 0:
        blinkMinInterval = 0.0f;
        blinkHoldTime = 86400.0f;
        blinkMaxInterval = 0.0f;
        blinkCloseTime = 0.0f;
        blinkOpenTime = 0.0f;
        break;
    case 1:
        blinkMinInterval = 2.0f;
        blinkMaxInterval = 4.0f;
        blinkHoldTime = blinkOpenTime = blinkCloseTime = 0.1f;
        break;
    case 6:
        blinkMinInterval = 2.0f;
        blinkMaxInterval = 4.0f;
        blinkCloseTime = 1.5f;
        blinkOpenTime = 0.5f;
        blinkHoldTime = 0.5f;
        break;
    case 2:
    case 3:
        blinkMinInterval = 1.0f;
        blinkMaxInterval = 2.0f;
        blinkHoldTime = blinkOpenTime = blinkCloseTime = 0.017f;
        break;
    case 4:
        blinkMinInterval = 0.5f;
        blinkMaxInterval = 2.0f;
        blinkHoldTime = blinkOpenTime = blinkCloseTime = 0.017f;
        break;
    case 5:
        blinkMinInterval = 2.0f;
        blinkMaxInterval = 6.0f;
        blinkHoldTime = blinkOpenTime = blinkCloseTime = 0.017f;
        break;
    case 8:
        blinkMinInterval = 0.0f;
        blinkHoldTime = 86400.0f;
        blinkMaxInterval = 0.0f;
        blinkCloseTime = 0.0f;
        blinkOpenTime = 0.0f;
        break;
    case 7:
        blinkMinInterval = 0.0f;
        blinkMaxInterval = 0.0f;
        blinkCloseTime = 86400.0f;
        blinkOpenTime = 0.0f;
        blinkHoldTime = 0.5f;
        break;
    default:
        blinkMinInterval = 2.0f;
        blinkMaxInterval = 5.0f;
        blinkHoldTime = blinkOpenTime = blinkCloseTime = 0.017f;
        break;
    }
    blinkTimer = 0.0f;
}
