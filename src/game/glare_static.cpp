// A fragment of the original glare code: CGlare's static calls. Names come from the symbol table.
#include "GlareManager.h"

void CGlare::InitClass() { g_manager.Init(); }

void CGlare::ResetClass() { g_manager.Reset(); }

void CGlare::ShutdownClass() { g_manager.Shutdown(); }

class ISceneNode;
class CScene {
  public:
    void Add(ISceneNode &, ISceneNode *);
};
extern CScene g_scene;

CGlare *CGlare::Create(int id, bool add) {
    CGlare *glare = g_manager.Create(id);
    if (add && glare)
        g_scene.Add(*(ISceneNode *)glare, 0);
    return glare;
}

// The command record a draw bin stores; only what Draw writes is named.
struct DrawCommand {
    unsigned char unknown00[0x0c];
    void *owner; // +0x0c
    unsigned char unknown10[0x1c - 0x10];
    void (*execute)(CDrawContext &, void **); // +0x1c
};

void CGlare::Draw(CDrawContext &context) {
    DrawCommand command;
    command.owner = this;
    command.execute = CGlareManager::ExecuteDraw;
    context.AddCommand((ERenderBins)10, command);
}
