// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AITargeting.h"

// Copies the point into the script data, then has locomotion track that copy.
void SetMovePoint(EMovePointType type, ISceneNode *node, CVector3 &point, float arrival) {
    AIScriptDataView *data = (AIScriptDataView *)((BSObject *)node->GetScriptObject())->gameObject->GetScriptData();
    data->movePoint.x = point.x;
    data->movePoint.y = point.y;
    data->movePoint.z = point.z;
    node->GetAIObject()->locomotion.SetupArbitraryPointUpdate(type, &data->movePoint, arrival);
}
