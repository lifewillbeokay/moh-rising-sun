// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BIFunc_AIMatchObjectID(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    BSObject *first = (BSObject *)*(*stack - (count - 1));
    BSObject *second = (BSObject *)*(*stack - (count - 2));
    int match = 0;
    if (first->nativeObject && second->nativeObject) {
        int id = first->nativeObject->GetID();
        if (id == second->nativeObject->GetID() && id)
            match = 1;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = match;
}
