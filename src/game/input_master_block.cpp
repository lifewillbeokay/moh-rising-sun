// A fragment of the original input code: blocking vibration for a player. Names as in Shell.h, inferred from what these methods do.
#include "Shell.h"

void CInputManager::SetMasterBlockVibrations(bool block, unsigned char player) {
    masterBlockVibrations[player] = block;
    if (block) {
        SetActuatorState(player, 0, 0, 0.0f, 0.0f);
        SetActuatorState(player, 0, 1, 0.0f, 0.0f);
    }
}
