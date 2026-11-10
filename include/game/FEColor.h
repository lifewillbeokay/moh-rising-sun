// FEColor: four integer channels, clamped when packed into a 32-bit colour.
#ifndef GAME_FECOLOR_H
#define GAME_FECOLOR_H
class FEColor {
  public:
    FEColor() {}
    FEColor(const FEColor &color) : b(color.b), g(color.g), r(color.r), a(color.a) {}
    FEColor(unsigned long);
    operator unsigned long() const;
    FEColor &operator=(const FEColor &);
    FEColor operator-(const FEColor &) const;
    int b, g, r, a; // +0x0, +0x4, +0x8, +0xc
};
#endif
