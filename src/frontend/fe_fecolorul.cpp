// FEColor: four integer channels, clamped when packed into a 32-bit colour.
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

FEColor::FEColor(unsigned long color) {
    a = color >> 24;
    r = (color >> 16) & 0xff;
    g = (color >> 8) & 0xff;
    b = color & 0xff;
}
FEColor::operator unsigned long() const {
    int red = r < 0 ? 0 : (r > 255 ? 255 : (unsigned char)r);
    int green = g < 0 ? 0 : (g > 255 ? 255 : (unsigned char)g);
    int blue = b < 0 ? 0 : (b > 255 ? 255 : (unsigned char)b);
    int alpha = a < 0 ? 0 : (a > 255 ? 255 : (unsigned char)a);
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}
FEColor &FEColor::operator=(const FEColor &color) {
    a = color.a;
    r = color.r;
    g = color.g;
    b = color.b;
    return *this;
}
