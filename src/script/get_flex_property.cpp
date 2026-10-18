// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "TriggerObject.h"
#include "ScriptRuntime.h"
#include "Weapon.h"

void BIFunc_GetFlexProperty(int **stack, void *) {
    TriggerObject *trigger = g_pBSObject->nativeObject;
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int field = args[1];
    switch (trigger->properties.flex.GetFieldType(field)) {
    case 2:
    case 8:
    case 9:
    case 10: {
        int value = (int)trigger->properties.flex.GetString(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = value;
        break;
    }
    case 7: {
        int value = trigger->properties.flex.GetEnum(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = value;
        break;
    }
    case 1: {
        int value = trigger->properties.flex.GetInt(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = value;
        break;
    }
    case 4: {
        float value = trigger->properties.flex.GetFloat(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        volatile float result = value;
        **stack = *(int *)&result;
        break;
    }
    case 5: {
        int value = trigger->properties.flex.GetBool(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = value;
        break;
    }
    case 6: {
        int value = (int)trigger->properties.flex.GetList(field);
        *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
        **stack = value;
        break;
    }
    }
}
