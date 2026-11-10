// FEMouse: pointer position, wheel and button state. Bit n of a mask is button n.
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

FEMouse::FEMouse() { Reset(); }
void FEMouse::Reset() {
    wheel = 0;
    y = 0;
    x = 0;
    current = 0;
    previous = 0;
    holdTime[2] = 0;
    holdTime[1] = 0;
    holdTime[0] = 0;
    unknown_1c = 0;
    moved = 0;
}
void FEMouse::Update(FEMouseInfo &info, unsigned long elapsed) {
    int changed = 0;
    if (info.x != x || info.y != y) changed = 1;
    moved = changed;
    unsigned short last = current;
    x = info.x;
    y = info.y;
    wheel = info.wheel;
    previous = last;
    current = info.buttons;
    for (int i = 0; i < 3; i++) {
        if ((current >> i) & 1) {
            if ((previous >> i) & 1) holdTime[i] += elapsed;
            else holdTime[i] = 0;
        }
    }
}
int FEMouse::WasPressed(unsigned short mask) {
    int result = 0;
    int down = current & mask;
    if (down == mask && (previous & down) != down) result = 1;
    return result;
}
int FEMouse::WasHeld(unsigned short mask) {
    bool result = false;
    int down = current & mask;
    if (down == mask) {
        int both = previous & down;
        result = both == down;
    }
    return result;
}
unsigned long FEMouse::HeldFor(unsigned short mask) {
    unsigned long shortest = 0xffffffff;
    for (int i = 0; i < 32; i++) {
        if ((mask >> i) & 1) {
            unsigned long held = holdTime[i];
            if (held > shortest) held = shortest;
            shortest = held;
        }
    }
    return shortest;
}
int FEMouse::WasReleased(unsigned short mask) {
    bool result = false;
    if ((current & mask) != mask) {
        int both = previous & mask;
        result = both == mask;
    }
    return result;
}
