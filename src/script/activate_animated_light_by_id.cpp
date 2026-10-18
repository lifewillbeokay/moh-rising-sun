// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "TriggerObject.h"
#include "Weapon.h"
#include "Light.h"

extern unsigned int g_pNumTriggers;
extern TriggerObject g_pTriggerObjects[];

void BIFunc_ActivateAnimatedLightByID(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = args[1];
    TriggerObject *trigger = g_pTriggerObjects;
    for (unsigned int i = 0; i < g_pNumTriggers; i++, trigger++) {
        if (trigger->IsAnimatedLight() && trigger->GetID() == id)
            CLight::Create((void *)trigger->properties.legacy);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
