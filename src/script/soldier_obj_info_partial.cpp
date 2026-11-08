// AI-assisted partial reconstruction from GR8E69; see docs/Script.md.
#if defined(DECOMP_COMPARE)
#include "SceneNode.h"
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
void GetObjInfo(int, BSObject *, float, ISceneNode *, bool &, int &, float &);
void BIFunc_Soldier_GetObjInfo(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int integer = 0;
    bool isFloat = false;
    float real = 0.0f;
    float result;
    BSObject *soldier = (BSObject *)*(*stack - (count - 1));
    int kind = *(*stack - (count - 2));
    BSObject *target = (BSObject *)*(*stack - (count - 3));
    float input = *(float *)(*stack - (count - 4));
    GetObjInfo(kind, target, input, soldier->gameObject->GetSceneNode(), isFloat, integer, real);
    if (isFloat) {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        result = real;
        **stack = *(volatile int *)&result;
    } else {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = integer;
    }
}

#endif
