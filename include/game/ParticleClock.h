#ifndef GAME_PARTICLE_CLOCK_H
#define GAME_PARTICLE_CLOCK_H

// Scoped static interface; no complete CPSManager storage declaration.
class IParticleSystem;
// Original enum tag; its members are unknown.
enum eParticleSystem { eParticleSystemDefault = 0 };
class CPSManager {
  public:
    static IParticleSystem *Create(unsigned int, bool, eParticleSystem);
    static void Update(float);
    static float GetCurrentTicks();
};

#endif
