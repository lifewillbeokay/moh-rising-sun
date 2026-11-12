// FEng controller state; shared from the verified FEJoyPad methods.
#ifndef GAME_FEJOYPAD_H
#define GAME_FEJOYPAD_H
class FEJoyPad {
  public:
    FEJoyPad();
    void Reset();
    void Update(unsigned long, unsigned long);
    int WasPressed(unsigned long);
    int WasHeld(unsigned long);
    unsigned long HeldFor(unsigned long);
    int WasReleased(unsigned long);
    void DecrementHold(unsigned long, unsigned long);

    unsigned long previous;     // +0x00
    unsigned long current;      // +0x04
    unsigned long holdTime[32]; // +0x08
};
#endif
