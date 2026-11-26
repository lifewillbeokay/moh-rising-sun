// AI-assisted GR8E69 reconstruction; see docs/research/save-popups.md.
#include "profile.h"
#include "SaveGamePopups.h"
#include "Camera.h"
#include "Screen.h"
#include "DrawContext.h"
#include "SoundManager.h"
#include <list>
// Method-only views of original global storage; no allocation or complete layout is implied.
class CViewport { public: void SetRect(float, float, float, float); };
class CScreenEAGL { public: CDrawContext *GetOrthographicDrawContext() const; };
extern CViewport g_viewport;
extern CScreenEAGL g_screen;
extern _STL::list<CSaveGamePopup *> g_listPopups;
extern int g_bBackgroundEnabled, g_bDisableSound;
extern float g_frameDel;
void Gameloop_Draw(float); void SoundSync(); void WaitScreen();
namespace GBALink { void Update(); }
extern "C" { void SNDSYS_entercritical(); void SNDSYS_leavecritical(); float SNDPROFILE_iopcpu(); void SYNCTASK_run(int); }
#ifdef DECOMP_COMPARE
void CSaveGamePopup::Wait() {
    if (g_bBackgroundEnabled) Gameloop_Draw(g_frameDel);
    g_inputMgr.Update(g_frameDel);
    if (g_listPopups.front()) g_listPopups.front()->Update(g_frameDel);
    g_viewport.SetRect(0.0f, 0.0f, (float)CScreen::m_sWidth, (float)CScreen::m_sHeight);
    g_camera.frustum.Set(g_camera, (CScreen &)g_screen, g_viewport);
    g_camera.frustum.Transform(g_camera.localToWorld);
    g_screen.GetOrthographicDrawContext()->Begin(0);
    _STL::list<CSaveGamePopup *>::reverse_iterator i = g_listPopups.rbegin(), end = g_listPopups.rend();
    for (; i != end; ++i) (*i)->Draw();
    if (!g_GameFlow.levelIsShell) { g_PauseScreen.UpdateUnplugged(); g_PauseScreen.DrawUnplugged(); }
    else { g_Shell.UpdateUnplugged(); g_Shell.DrawUnplugged(); }
    g_screen.GetOrthographicDrawContext()->End();
    if (!g_bDisableSound) {
        SNDSYS_entercritical(); g_SoundMan.ServiceStreams(); g_SoundMan.schedule.Update(1.0f); SNDSYS_leavecritical();
        if (g_SoundMan.field_86f8 % 20 == 0) g_SoundMan.field_86f4 = SNDPROFILE_iopcpu();
        ++g_SoundMan.field_86f8;
    }
    SYNCTASK_run(0); SoundSync(); GBALink::Update(); WaitScreen();
}
#endif
