// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "AITargeting.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
namespace AI { void Log(CAIObject *, char *, ...); }
void CAITargeting::SetNonAITarget(BSObject *object) {
    target.Nullify();
    if (object) {
        target.SetAsNonAITarget(object);
        WeakPtr<BSGO_Basic, 8> &gameObject = object->gameObject;
        if (gameObject && gameObject->GetSceneNode()) {
            gameObject->GetSceneNode()->GetPosition(targetPosition);
            modifiedPosition = targetPosition;
            lastSeenPosition = targetPosition;
            lastSeenSet = 1;
        } else if (object->nativeObject) {
            object->nativeObject->GetPosition(targetPosition);
            modifiedPosition = targetPosition;
            lastSeenPosition = targetPosition;
            lastSeenSet = 1;
        }
        AI::Log(sceneNode->GetAIObject(), "Setting Non-AI Target at position %f %f %f\n",
                modifiedPosition.x, modifiedPosition.y, modifiedPosition.z);
    }
}
