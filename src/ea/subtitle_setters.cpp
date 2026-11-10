// A fragment of the original subtitle code: one subtitle's defaults and setters. Names come from the symbol table;
// members are inferred from the code.
namespace RCMP {
class AV_SUBTITLE {
  public:
    enum JUSTIFY_ENUM { JUSTIFY_UNKNOWN };
    AV_SUBTITLE();
    void SetPosX(int);
    void SetPosY(int);
    void SetWidth(unsigned int);
    void SetHeight(unsigned int);
    void SetStrID(unsigned int);
    void SetFrameNumber(unsigned int);
    void SetJustifyX(JUSTIFY_ENUM);
    void SetJustifyY(JUSTIFY_ENUM);
    void SetShapeIndex(unsigned int);
    void SetA(unsigned int);
    void SetR(unsigned int);
    void SetG(unsigned int);
    void SetB(unsigned int);
    int posX;
    int posY;
    unsigned int width;
    unsigned int height;
    unsigned int strID;       // +0x10
    unsigned int frameNumber; // +0x14
    JUSTIFY_ENUM justifyX;    // +0x18
    JUSTIFY_ENUM justifyY;    // +0x1c
    unsigned int shapeIndex;  // +0x20
    unsigned char a, r, g, b; // +0x24
    unsigned int unknown28;
    unsigned int unknown2c;
    char *wrapped;            // +0x30
};

AV_SUBTITLE::AV_SUBTITLE() {
    strID = 0xffffffff;
    a = 255;
    posX = 32;
    posY = 350;
    width = 600;
    height = 50;
    r = 255;
    g = 255;
    frameNumber = 0xffffffff;
    b = 255;
    justifyX = (JUSTIFY_ENUM)2;
    shapeIndex = 0;
    unknown28 = 0;
    unknown2c = 0;
    justifyY = (JUSTIFY_ENUM)2;
    wrapped = 0;
}

void AV_SUBTITLE::SetPosX(int value) { posX = value; }
void AV_SUBTITLE::SetPosY(int value) { posY = value; }
void AV_SUBTITLE::SetWidth(unsigned int value) { width = value; }
void AV_SUBTITLE::SetHeight(unsigned int value) { height = value; }
void AV_SUBTITLE::SetStrID(unsigned int value) { strID = value; }
void AV_SUBTITLE::SetFrameNumber(unsigned int value) { frameNumber = value; }
void AV_SUBTITLE::SetJustifyX(JUSTIFY_ENUM value) { justifyX = value; }
void AV_SUBTITLE::SetJustifyY(JUSTIFY_ENUM value) { justifyY = value; }
void AV_SUBTITLE::SetShapeIndex(unsigned int value) { shapeIndex = value; }
void AV_SUBTITLE::SetA(unsigned int value) { a = value; }
void AV_SUBTITLE::SetR(unsigned int value) { r = value; }
void AV_SUBTITLE::SetG(unsigned int value) { g = value; }
void AV_SUBTITLE::SetB(unsigned int value) { b = value; }
}
