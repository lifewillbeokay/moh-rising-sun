// AI-assisted reconstruction from GR8E69; see docs/Behaviors.md. The original file name is not known.
#include "SceneObjects.h"
#include "AIBot.h"

extern int g_NumBotsCreated;

// The clock runs in sixtieths of a second of its input; the first update with a bot created runs Init.
void CBotMan::Update(float frames) {
    if (!resetFlag && g_NumBotsCreated > 0) {
        Init();
        resetFlag = 1;
    }
    clock += frames * 0.016666668f;
}
