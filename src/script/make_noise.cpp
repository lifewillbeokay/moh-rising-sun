// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "CVector3.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void MakeNoise(void *, CAIObject *, const CVector3 &, int, float, bool, int);
void MakeNoise(ISceneNode *, int, float, int);

void BIFunc_MakeNoise(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int loud = *(*stack - (count - 1));
    float radius = *(float *)(*stack - (count - 2));
    int type = 49;
    if (!loud)
        type = 48;
    if (g_pBSObject) {
        if (g_pBSObject->gameObject && g_pBSObject->gameObject->GetSceneNode())
            MakeNoise(g_pBSObject->gameObject->GetSceneNode(), type, radius, type == 48 ? 1 : 3);
        else if (g_pBSObject && g_pBSObject->nativeObject) {
            CVector3 position;
            g_pBSObject->nativeObject->GetPosition(position);
            MakeNoise(object, 0, position, type, radius, true, type == 48 ? 1 : 3);
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
