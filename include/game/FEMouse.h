// FEMouse: pointer position, wheel and button state. Bit n of a mask is button n.
#ifndef GAME_FEMOUSE_H
#define GAME_FEMOUSE_H
struct FEMouseInfo {
    short x;               // +0
    short y;               // +2
    short wheel;           // +4
    unsigned short buttons; // +6
};
class FEMouse {
  public:
    FEMouse();
    void Reset();
    void Update(FEMouseInfo &, unsigned long);
    int WasPressed(unsigned short);
    int WasHeld(unsigned short);
    unsigned long HeldFor(unsigned short);
    int WasReleased(unsigned short);

    int x;                       // +0x00
    int y;                       // +0x04
    int wheel;                   // +0x08
    unsigned short previous;     // +0x0c
    unsigned short current;      // +0x0e
    unsigned long holdTime[3];   // +0x10
    int unknown_1c;              // +0x1c
    int moved;                   // +0x20
};
#endif
