// AI-assisted reconstruction of the particle system's transform setters; see docs/ParticleRecipes.md.
#include "ParticleRecipe.h"
// Rotates a vector by the matrix's upper 3x3, ignoring translation. Name is descriptive.
static inline void RotateVector(CVector3 &out, const CMatrix &m, const CVector3 &v) {
    CVector3 r;
    r.x = v.x * m.row[0].x + v.y * m.row[1].x + v.z * m.row[2].x;
    r.y = v.x * m.row[0].y + v.y * m.row[1].y + v.z * m.row[2].y;
    r.z = v.x * m.row[0].z + v.y * m.row[1].z + v.z * m.row[2].z;
    out = r;
}
// Re-expresses the definition's acceleration and the system's velocity vectors in local space.
static inline void UpdateLocalVectors(CParticleSystem *system) {
    CMatrix inverse;
    CMatrix *p = &inverse;
    if (!CMatrix::s_ClassInit)
        CMatrix::InitClass();
    CVector3 gravity;
    LevelFileContentsStruct_ *contents = system->definition->contents;
    gravity.Set(contents->entries[19]->value.scalar, contents->entries[20]->value.scalar, contents->entries[21]->value.scalar);
    p->FastInverse(system->localToWorld);
    RotateVector(system->particleAcceleration, *p, gravity);
    RotateVector(system->acceleration, *p, system->acceleration);
    RotateVector(system->initialVelocity, *p, system->initialVelocity);
}
void CParticleSystem::SetTMLocalToWorld(const CMatrix &m) {
    localToWorld = m;
    UpdateLocalVectors(this);
}
void CParticleSystem::PreTransform(const CMatrix &m) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(m, CMatrix::s_TempMat);
    UpdateLocalVectors(this);
}
void CParticleSystem::Transform(const CMatrix &m) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(CMatrix::s_TempMat, m);
    UpdateLocalVectors(this);
}
void CParticleSystem::Move(const CVector3 &offset) {
    localToWorld.Translate(offset);
    UpdateLocalVectors(this);
}
void CParticleSystem::Rotate(const CVector3 &axis, float angle) {
    localToWorld.BuildRot(axis, angle);
    UpdateLocalVectors(this);
}
void CParticleSystem::Pitch(float angle) {
    localToWorld.BuildRotX(angle);
    UpdateLocalVectors(this);
}
void CParticleSystem::Roll(float angle) {
    localToWorld.BuildRotY(angle);
    UpdateLocalVectors(this);
}
void CParticleSystem::Yaw(float angle) {
    localToWorld.BuildRotZ(angle);
    UpdateLocalVectors(this);
}
void CParticleSystem::SetPosition(const CVector3 &position) {
    localToWorld.SetPos(position);
    UpdateLocalVectors(this);
}
void CParticleSystem::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    localToWorld.SetRight(right);
    localToWorld.SetFront(front);
    localToWorld.SetUp(up);
    UpdateLocalVectors(this);
}
