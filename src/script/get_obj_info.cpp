// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"

void GetObjInfo(int, BSObject *, float, ISceneNode *, bool &, int &, float &);

void BIFunc_GetObjInfo(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int integer = 0;
    bool isFloat = false;
    float real = 0.0f;
    float result;
    int kind = *(*stack - (count - 1));
    BSObject *target = (BSObject *)*(*stack - (count - 2));
    float input = *(float *)(*stack - (count - 3));
    GetObjInfo(kind, target, input, (ISceneNode *)object, isFloat, integer, real);
    if (isFloat) {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        result = real;
        **stack = *(volatile int *)&result;
    } else {
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = integer;
    }
}
