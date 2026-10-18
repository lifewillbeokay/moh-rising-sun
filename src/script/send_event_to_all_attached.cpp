// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "ScriptTimers.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_SendEventToAllAttached(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *object = (BSObject *)*(*stack - (count - 1));
    int event = *(*stack - (count - 2));
    CStaticObjectInterface *parent = (CStaticObjectInterface *)object->gameObject->GetSceneNode()->AsStaticObject();
    if (parent->GetObjectType() == 1) {
        CAttachableObject *attachable = (CAttachableObject *)parent;
        for (int handle = attachable->GetFirstChildHandle(); handle != -1; handle = attachable->GetNextChildHandle(handle)) {
            ISceneNode *child = attachable->GetChild(handle);
            if (child)
                BSObjectTriggerEvent((BSObject *)child->GetScriptObject(), event, 0, 0, true);
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
