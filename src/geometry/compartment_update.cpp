// A fragment of the original compartment code: texture swaps and per-frame animation. Names come from
// the symbol table; the primer table's members are inferred from this code.
#include "Compartment.h"

class AnimShape {
  public:
    void StepToNextFrame();
    void Update(float);
    char name[5];
    unsigned char state; // +0x05; '-' is the default state.
    unsigned char unknown06[60 - 6];
};
class MaterialAnim {
  public:
    void Update(float);
};
struct PrimerTextureRef {
    unsigned char unknown00[48];
        int shapeIndex; // +0x30; an index into the swap shapes, or negative.
};
struct PrimerMaterialAnims {
      int count;
    MaterialAnim *anims[1]; // +0x04; extent is count.
};
class CPTHeader {
  public:
    unsigned char unknown00[36];
    int *textures; // +0x24; a count, then two words per entry with the reference first.
    unsigned char unknown28[60 - 40];
    PrimerMaterialAnims *materialAnims; // +0x3c
};

int CCompartment::DoTextureSwap(int index) {
    int *table = primerTable->textures;
    int count = *table;
    if (index >= 0 && index < count) {
        int offset = index * 8 + 4;
        int shape = (*(PrimerTextureRef **)((char *)table + offset))->shapeIndex;
        if (shape >= 0 && shape < swapShapes.finish - swapShapes.start) {
            AnimShape *animShape = &swapShapes.start[shape];
            if (animShape->state != '-') {
                animShape->StepToNextFrame();
                return 1;
            }
        }
    }
    return 0;
}

void CCompartment::HandleExplosion(const CExplosion &) {}

void CCompartment::BeginUpdate(float elapsed) {
    for (AnimShape *shape = animShapes.begin(); shape != animShapes.end(); shape++)
        shape->Update(elapsed);
    if (primerTable->materialAnims) {
        for (int i = 0; i < primerTable->materialAnims->count; i++)
            primerTable->materialAnims->anims[i]->Update(elapsed);
    }
}
