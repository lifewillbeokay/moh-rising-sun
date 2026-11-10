// CCharacterShadow's constructor and destructor, which holds the class table.
#include "CharacterShadow.h"

CCharacterShadow::CCharacterShadow() {
    enabled = 1;
    scale = 1.0f;
    field18 = 0.0f;
    caster = 0;
    field10 = 0;
    projector = 0;
    field20 = 0;
    InitData();
}

CCharacterShadow::~CCharacterShadow() {
    Release();
}
