// CCharacterShadow: dropping the projector and the empty InitClass.
#include "CharacterShadow.h"

class CProjector {
public:
    static int Destroy(CProjector *);
    unsigned char unknown000[592];
    int owner;
};

void CCharacterShadow::Release() {
    if (projector) {
        projector->owner = 0;
        CProjector::Destroy(projector);
        projector = 0;
    }
    field20 = 0;
    field10 = 0;
}

void CCharacterShadow::InitClass() {}
