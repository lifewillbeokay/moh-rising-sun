// AI-assisted reconstruction from GR8E69. A fragment of the original bot object source: the
// original file name is not known. Method names come from the symbol table; the attribute
// members are numbered because their meanings are not established.
#include "AIBot.h"

CBotCharacter::CBotCharacter() { Init(); }

void CBotCharacter::Init() {
    attributes[0] = 1.0f;
    attributes[1] = 1.0f;
    attributes[2] = 1.0f;
    attributes[3] = 1.0f;
    attributes[4] = 1.0f;
    attributes[5] = 1.0f;
    attributes[6] = 1.0f;
    field48 = 0.5f;
    field4c = 1;
    field50 = 1;
    field54 = 1.0f;
    field58 = 4.0f;
}
