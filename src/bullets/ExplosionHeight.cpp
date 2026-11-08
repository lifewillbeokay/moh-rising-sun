// AI-assisted partial reconstruction from GR8E69.
// See docs/research/explosion-grenade-updates.md.
#include "Bullet.h"

#if defined(DECOMP_COMPARE)
// Descriptive raw-component view; preserves the original vector-copy lifetimes.
static inline float Z(const CVector3 &v) {
    return *reinterpret_cast<const float *>(reinterpret_cast<const char *>(&v) + 8);
}

bool CExplosion::ShouldHeightCheckBeFlipped(ISceneNode &node) {
    switch (bulletType) {
    case 21:
    case 22:
    case 23: {
        CVector3 position, center;
        node.GetPosition(position);
        center = sphere.GetCenter();
        if (Z(center) - Z(position) > 2.0f)
            return true;
    }
    }
    return false;
}
#endif
