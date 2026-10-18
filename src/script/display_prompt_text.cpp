// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CVector3.h"
#include <stdio.h>
#include "SceneNode.h"
#include "CameraShake.h"
#include "Objectives.h"

extern CPlayerObject *g_pTriggeringPlayer;
extern int g_prompt;
extern char *g_promptCursor;
extern char g_promptBuffer[20][80];

void BIFunc_DisplayPromptText(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int message = *(*stack - (count - 1));
    int show = *(*stack - (count - 2));
    const char *text = g_pPopUpMessageHandler->messages.GetString(message);
    if (text) {
        int slot = -1;
        if (object) {
            CVector3 position;
            ((ISceneNode *)object)->GetPosition(position);
            slot = g_scene.FindClosestPlayer(position)->promptSlot;
        } else if (g_pTriggeringPlayer)
            slot = g_pTriggeringPlayer->promptSlot;
        appendMessageToPrompt(text, slot);
        if (show && g_promptCursor != g_promptBuffer[g_prompt])
            displayPromptBuffer(slot, -1.0f);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DisplayPromptInteger(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int value = *(*stack - (count - 1));
    int show = *(*stack - (count - 2));
    char text[32];
    int slot = -1;
    sprintf(text, "%d", value);
    if (object) {
        CVector3 position;
        ((ISceneNode *)object)->GetPosition(position);
        slot = g_scene.FindClosestPlayer(position)->promptSlot;
    } else if (g_pTriggeringPlayer)
        slot = g_pTriggeringPlayer->promptSlot;
    appendMessageToPrompt(text, slot);
    if (show && g_promptCursor != g_promptBuffer[g_prompt])
        displayPromptBuffer(slot, -1.0f);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
