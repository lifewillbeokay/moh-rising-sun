// A fragment of the original objectives file: saving each objective's state. Names as in Objectives.h.
#include "Objectives.h"
#include "GameSettings.h"

void CPlayerObjectives::Serialize() {
    int count = 10;
    g_GameSettings.SerializeValue(count, 6);
    for (unsigned int i = 0; i <= 9; i++) {
        g_GameSettings.SerializeValue((bool &)objectives[i].completed);
        g_GameSettings.SerializeValue((bool &)objectives[i].shown);
    }
}
