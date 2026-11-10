// A fragment of the original sprite code: texture coordinates scaled to a sprite. Names come from the symbol table; the
// return type's name and the scale members are inferred.
struct _spritepolyvert {
    float s, t;
};
struct COORD4 {
    float x, y, z, w;
};
class CSprite {
  public:
    COORD4 STQ(float, float) const;
    COORD4 STQ(const _spritepolyvert &) const;
    unsigned char unknown00[0x10];
    float scaleX; // +0x10
    float scaleY; // +0x14
};

COORD4 CSprite::STQ(float s, float t) const {
    COORD4 coord;
    coord.x = s * scaleX;
    coord.y = t * scaleY;
    coord.z = 1.0f;
    coord.w = 1.0f;
    return coord;
}

COORD4 CSprite::STQ(const _spritepolyvert &vertex) const {
    COORD4 coord;
    coord.x = vertex.s * scaleX;
    coord.y = vertex.t * scaleY;
    coord.z = 1.0f;
    coord.w = 1.0f;
    return coord;
}
