// AI-assisted reconstruction of the mesh particle system's draw; see docs/ParticleRecipes.md.
#include "DrawContext.h"
#include "ParticleRecipe.h"
// The command record a draw bin stores; only what Draw writes is named.
class DrawCommand {
  public:
    int sortKey;
    CStaticMesh *mesh;
    unsigned char unknown_8[4];
    void *owner;
    unsigned char unknown_10[0x1c - 0x10];
    void (*execute)(CDrawContext &, void **);
    CVector3 offset;
};
// A camera's matrix sits at +0x50 of the object the context holds.
struct DrawView {
    unsigned char unknown_0[0x50];
    CMatrix matrix;
};
static inline void SetDiff(CVector3 &r, const CVector3 &a, const CVector3 &b) {
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
}
void MeshParticleSystem::Draw(CDrawContext &context) {
    if (flags.running) {
        DrawCommand command;
        SetDiff(command.offset, localToWorld.GetPos(), ((DrawView *)context.sceneCamera)->matrix.GetPos());
        command.sortKey = (int)command.offset.LengthSquared();
        command.mesh = definition->mesh;
        command.execute = DrawParticles;
        command.owner = this;
        context.AddCommand((ERenderBins)9, command);
    }
}
