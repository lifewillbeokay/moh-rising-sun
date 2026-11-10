// AI-assisted reconstruction of the character shadow's projector setup; see docs/Rendering.md.
#include "CharacterShadow.h"
#include "Projector.h"
unsigned int AssetCRC(char *);
void CCharacterShadow::InitProjector() {
    CProjector *existing = projector;
    if (!existing) {
        char name[] = "npcshadow";
        projector = CProjector::Create(AssetCRC(name), true);
        if (!projector) {
            field20 = projector;
            field10 = projector;
        } else {
            projector->enabled250 = 0;
        }
    }
}
