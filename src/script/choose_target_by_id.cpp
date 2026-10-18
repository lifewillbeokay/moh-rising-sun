// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

extern int g_bInMultiplayerMode;

// A null target clears both target kinds. In single player an AI target is set
// directly and anything else becomes a script target; in multiplayer only AI
// targets are accepted. The targeting address test is kept as observed.
int ChooseTargetByID(CAIObject *ai, BSObject *target) {
    int chosen = 0;
    if (!target) {
        ai->targeting.SetTarget(0);
        ai->targeting.SetNonAITarget(0);
    } else {
        CAIObject *other;
        if (target->gameObject && target->gameObject->GetSceneNode())
            other = target->gameObject->GetSceneNode()->GetAIObject();
        else
            other = 0;
        if (!g_bInMultiplayerMode) {
            if (other)
                ai->targeting.SetTarget(other);
            else
                ai->targeting.SetNonAITarget(target);
            chosen = 1;
        } else if (other && &ai->targeting) {
            ai->targeting.SetTarget(other);
            chosen = 1;
        } else
            chosen = 0;
    }
    return chosen;
}
