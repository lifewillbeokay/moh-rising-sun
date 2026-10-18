// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WORLD_OBJECT_H
#define GAME_WORLD_OBJECT_H

// Method-only view of the world (compartment) object held by CScene at +0x18.
class CWorldObject {
  public:
    // Sixteen-byte compartment request; the four entries are sign-extended shorts.
    struct SAcquireSet {
        int compartments[4];
    };
    void Acquire(const SAcquireSet &, bool, bool);
};

#endif
