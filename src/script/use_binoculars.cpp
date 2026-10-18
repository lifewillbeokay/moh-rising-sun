// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include <stdio.h>
#include <string.h>
#include "SceneNode.h"
#include "CameraShake.h"

void BIFunc_UseBinoculars(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    bool use = *(bool *)(*stack - (count - 1));
    char *name = (char *)*(*stack - (count - 2));
    g_scene.GetPlayer(0)->pathBits.binoculars = use;
    g_scene.GetPlayer(0)->SetHideWeapon(use);
    g_scene.GetPlayer(0)->FreeBinocularOverlay();
    if (use) {
        char *extension = strchr(name, '.');
        if (extension)
            *extension = 0;
        sprintf(name, "%s.%s", name, "gsh");
        CSprite *sprite = new ("source/ai_script/bsbifunc.cpp:10239") CSprite(name, CSprite::SpriteLoadUnknown1);
        sprite->field_1c = 0;
        sprite->field_20 = 0;
        g_scene.GetPlayer(0)->binocularOverlay = sprite;
        g_scene.GetPlayer(0)->SetZoom(15.0f);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
