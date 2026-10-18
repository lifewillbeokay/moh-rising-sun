// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_FadeOut(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)(*stack - (count - 1));
    switch (*(*stack - (count - 2))) {
    case 2:
        ((CAnimObjectInterface *)object)->FadeOut(time);
        break;
    case 5:
        ((CStaticObject *)object)->FadeOut(time);
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_FadeIn(int **stack, void *object) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float time = *(float *)(*stack - (count - 1));
    switch (*(*stack - (count - 2))) {
    case 2:
        ((CAnimObjectInterface *)object)->FadeIn(time);
        break;
    case 5:
        ((CStaticObject *)object)->FadeIn(time);
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
