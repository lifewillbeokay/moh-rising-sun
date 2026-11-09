#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H
// A scoped view of the draw context. The two matrix pointers are named from how the sky box
// uses them.
#include "CMatrix.h"

enum ERenderBins { RenderBinUnknown = 0 };

class DrawCommand;
struct COORD3;

class CCamera;
class CFog;
class CDrawContext {
public:
    unsigned char unknown000[192];
    CMatrix *modelView;
    CMatrix *camera;
    unsigned char unknown0c8[212 - 200];
    CCamera *sceneCamera; // +0xd4
    CFog *fog;            // +0xd8
    unsigned char unknown0dc[240 - 220];
    void *visibleLists[3]; // +0xf0: the scene's vectors filled before a draw
    unsigned char unknown0fc[4];
    unsigned int clipFlags; // +0x100, cleared by CVolBox::TestVisibility
    unsigned char unknown104[548 - 260];
    void AddCommand(ERenderBins bin, const DrawCommand &command);
    void InitCommands();
    void ExecCommands();
    // The table pointer follows the fields, at +0x224. The table holds the destructor and four
    // pure virtual slots. Only the last slot's use is observed, by CStaticMesh::IsVisible, and its
    // name is inferred from that use.
    virtual ~CDrawContext();
    // Bracket a draw: slot names inferred from CStaticScreen::DrawLoadingText.
    virtual void Begin(int) = 0;
    virtual void End() = 0;
    virtual void PureSlot3() = 0;
    virtual int IsSphereVisible(const COORD3 &center, float radius) = 0;
};

#endif
