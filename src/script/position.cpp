// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"
#include "SceneNode.h"
void GetScriptPosition(BSObject *object, ISceneNode *node, CVector3 &out) {
    if (!node && object && object->gameObject)
        node = object->gameObject->GetSceneNode();
    if (node)
        node->GetPosition(out);
    else if (object && object->nativeObject)
        object->nativeObject->GetPosition(out);
}
