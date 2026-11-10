// AI-assisted reconstruction of the decal manager's draw submission; see docs/Rendering.md.
#include "DecalManager.h"
#include "DrawContext.h"
int CDecalManager::IsVisible(CDrawContext &) const { return 1; }
unsigned int CDecalManager::IsDrawEnabled() const { return 1; }
// The command record a draw bin stores; only what Draw writes is named.
class DrawCommand {
  public:
    unsigned char unknown_0[12];
    void *owner;
    unsigned char unknown_10[0x1c - 0x10];
    void (*execute)(CDrawContext &, void **);
};
void CDecalManager::Draw(CDrawContext &context) {
    DrawCommand command;
    command.owner = this;
    command.execute = ExecuteDraw;
    context.AddCommand((ERenderBins)7, command);
}
