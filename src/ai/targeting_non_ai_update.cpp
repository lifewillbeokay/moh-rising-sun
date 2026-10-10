// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIFilterGlobal.h"
void CAITargeting::UpdateNonAITargetPosition() {
    if (target.scriptObject) {
        WeakPtr<BSGO_Basic, 8> &gameObject = target.scriptObject->gameObject;
        if (gameObject && gameObject->GetSceneNode()) {
            gameObject->GetSceneNode()->GetPosition(targetPosition);
            modifiedPosition = targetPosition;
            lastSeenPosition = targetPosition;
            lastSeenSet = 1;
            AdjustAimPosition(&modifiedPosition, weapon, &aimMode);
            targetUpdateTime = g_aigAIFilterGlobalObject.time;
        }
    }
}
