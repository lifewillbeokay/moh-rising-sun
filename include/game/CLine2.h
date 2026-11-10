#ifndef GAME_CLINE2_H
#define GAME_CLINE2_H

// AI-assisted reconstruction; see docs/Geometry.md. Two floats are independently
// copied as an eight-byte value. Inline names and member names are descriptive.
struct CVector2 {
    float x, y;

    void Set(float ax, float ay) {
        x = ax;
        y = ay;
    }
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
    CLine2() {}
    CLine2(const CVector2& a, const CVector2& b) : start(a), end(b) {}
    CLine2& operator=(const CLine2& other) {
        if (&other != this) {
            start = other.start;
            end = other.end;
        }
        return *this;
    }
    float GetClosestPointSquared(const CVector2&, CVector2*) const;
    int Intersect(const CLine2&, float, CVector2*) const;
    int IsDegenerate() const;
};

#endif
