// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_LetterboxTransition(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int time = *(*stack - (count - 1));
    _LetterboxTransitionStyle style = (_LetterboxTransitionStyle)*(*stack - (count - 2));
    g_scene.GetPlayer(0)->hud.LetterboxTransition(time, style);
}
