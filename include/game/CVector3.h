// AI-assisted reconstruction; see docs/Matrix.md, "CVector3 layout".
#ifndef GAME_CVECTOR3_H
#define GAME_CVECTOR3_H

// Sixteen bytes: three components and a fourth word that every constructor sets
// to 1.0. Member names and the inline helpers are descriptive reconstruction
// choices; the constructor and assignment shapes are those that reproduce the
// original temporaries. Assignment copies only x, y and z and returns by value.
// Default construction sets only the fourth word.
class CVector3 {
  public:
    float x, y, z, w;
    CVector3() : w(1.0f) {}
    CVector3(float ax, float ay, float az) : x(ax), y(ay), z(az), w(1.0f) {}
    CVector3(const CVector3 &o) : x(o.x), y(o.y), z(o.z), w(1.0f) {}
    CVector3 operator=(const CVector3 &o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
    CVector3 &operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    CVector3 &operator+=(const CVector3 &o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    CVector3 &operator-=(const CVector3 &o) {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    void Set(float ax, float ay, float az) {
        x = ax;
        y = ay;
        z = az;
    }
    float LengthSquared() const { return x * x + y * y + z * z; }
    float LengthSquaredXY() const { return x * x + y * y; }

    void Constrain(CVector3 &, float);
    void RotateAboutX(float);
    void RotateAboutZ(float);
    float Distance(const CVector3 &) const;
    float DistanceXY(const CVector3 &) const;
    float DistanceSquared(const CVector3 &) const;
    float DistanceSquaredXY(const CVector3 &) const;
    float Dot(const CVector3 &) const;
};

typedef char CVector3StorageSizeCheck[sizeof(CVector3) == 16 ? 1 : -1];

#endif
