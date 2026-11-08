// AI-assisted partial reconstruction from GR8E69; see docs/Script.md.
#if defined(DECOMP_COMPARE)
#include "SceneObjects.h"
#include "ScriptBuiltins.h"
void BIFunc_SetExpression(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int expression = *(*stack - (count - 1));
    float time = *(float *)(*stack - (count - 2));
    CSoldierObject *soldier = ((ISceneNode *)object)->AsSoldierObject();
    if (soldier->expression.pending >= 0) {
        soldier->expression.currentTime = time;
        soldier->expression.pending = -1;
        soldier->expression.currentBlend = time;
        soldier->expression.current = expression;
        soldier->expression.pendingBlend = 0.0f;
        soldier->expression.currentRate = 0.0f;
        soldier->expression.pendingTime = 0.0f;
        soldier->expression.pendingRate = 0.0f;
    } else if (soldier->expression.current == -1) {
        soldier->expression.currentTime = time;
        soldier->expression.currentRate = time * 0.25f;
        soldier->expression.current = expression;
        soldier->expression.currentBlend = 0.0f;
    } else if (soldier->expression.current != expression) {
        soldier->expression.pendingTime = time;
        soldier->expression.pendingRate = time * 0.25f;
        soldier->expression.currentRate = -soldier->expression.currentBlend * 0.25f;
        soldier->expression.pending = expression;
        soldier->expression.pendingBlend = 0.0f;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = -1;
}

#endif
