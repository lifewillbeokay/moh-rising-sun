// A fragment of the original AnimObjFactory.cpp: setting up a player weapon's definition from the
// level's contents. Class and method names come from the symbol table; the definition's members and
// the table indices' meanings are inferred from this code.
#include "Toilet.h"

class CAnimObjDef {
  public:
    unsigned char unknown00[32];
    int value20; // +0x20
    int value24; // +0x24
    int value28; // +0x28
    unsigned char unknown2c[8];
    int value34; // +0x34
    unsigned char unknown38[12];
    LevelFileContentsStruct_ *contents; // +0x44
    unsigned char unknown48[0x60 - 0x48];
};

class CAnimObjectFactory {
  public:
    void LoadPlayerWeapon(LevelFileContentsStruct_ *contents, CAnimObjDef *definition);
    void InitPlayerWeapon(LevelFileContentsStruct_ *contents, int index);
    CAnimObjDef *definitions; // +0x00
};

void CAnimObjectFactory::InitPlayerWeapon(LevelFileContentsStruct_ *contents, int index) {
    definitions[index].contents = contents;
    definitions[index].value34 = contents->data[9]->value;
    definitions[index].value24 = -1;
    LoadPlayerWeapon(contents, &definitions[index]);
    definitions[index].value20 = -1;
    definitions[index].value28 = 0;
}
