// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneNode.h"
#include "AIBehavior.h"

void BIFunc_SignalBuddyLeadCheckpointReached(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int checkpoint = *(*stack - (count - 1));
    BSObject *buddy = (BSObject *)*(*stack - (count - 2));
    if (buddy && buddy->gameObject) {
        ISceneNode *node = buddy->gameObject->GetSceneNode();
        if (node) {
            CAIObject *ai = node->GetAIObject();
            if (ai) {
                CBuddyLeadBehavior *lead = (CBuddyLeadBehavior *)ai->FindMatchingBehavior(BehaviorTypeBuddyLead);
                if (lead)
                    lead->HandleCheckpointReachedByBuddy(checkpoint);
            }
        }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
