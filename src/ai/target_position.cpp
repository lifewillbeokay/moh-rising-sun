// AI-assisted reconstruction from GR8E69; see docs/Targeting.md.
#include "ScriptRuntime.h"
#include "AITargeting.h"
#include "SceneNode.h"
CVector3 CAITarget::GetPosition() const {
    if (nonAI) {
        CVector3 result;
        if (scriptObject->gameObject && scriptObject->gameObject->GetSceneNode())
            scriptObject->gameObject->GetSceneNode()->GetPosition(result);
        else
            scriptObject->nativeObject->GetPosition(result);
        return result;
    }
    return aiObject->position;
}
