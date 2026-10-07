#ifndef GAME_CLINE2_H
#define GAME_CLINE2_H

// AI-assisted reconstruction; see docs/Geometry.md. Two floats are independently
// copied as an eight-byte value. Inline names and member names are descriptive.
struct CVector2 {
    float x, y;

    CVector2& operator-=(const CVector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    CVector2& operator+=(const CVector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    CVector2& operator*=(float scale) {
        x *= scale;
        y *= scale;
        return *this;
    }
    float Dot(const CVector2& other) const {
        return x * other.x + y * other.y;
    }
    float LengthSquared() const { return x * x + y * y; }
};

typedef char CVector2StorageSizeCheck[sizeof(CVector2) == 8 ? 1 : -1];

// Observed endpoint prefix; do not infer a complete class allocation from it.
class CLine2 {
public:
    CVector2 start, end;
    float GetClosestPointSquared(const CVector2&, CVector2*) const;
};

#endif
