// AI-assisted reconstruction from GR8E69. Fragments of the original AISoldierObject.cpp;
// method names come from the symbol table.
#include "AIBehavior.h"

void CAISoldierObject::OnStateChange(EAIHighLevelState previous, EAIHighLevelState next) {
    threatEvaluation.OnStateChange(previous, next);
}

void CAISoldierObject::OnDeath() {
    for (CAIObject *other = CAIObject::GetGlobalListStart(); other; other = other->nextGlobal) {
        CAISoldierObject *soldier = ((CAIObjectInterface *)other)->AsAISoldierObject();
        if (soldier)
            soldier->threatEvaluation.AIDied(((CAIObject *)this)->scriptObject);
    }
}
