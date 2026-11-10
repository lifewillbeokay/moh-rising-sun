// AI-assisted reconstruction of the buddy lead checkpoints; see docs/Behaviors.md. Names come from the symbol table.
#include "profile.h"
#include "AIBehavior.h"
#include "TriggerObject.h"
#include <map>
typedef _STL::map<int, Checkpoint_t *> CheckpointMap;
extern CheckpointMap g_checkpointMap;

static Checkpoint_t *GetNextCheckpoint(Checkpoint_t *);

void CBuddyLeadBehavior::HandleCheckpointReachedByBuddy(int id) {
    Checkpoint_t *reached = g_checkpointMap[id];
    if (reached == currentCheckpoint) {
        Checkpoint_t *next = GetNextCheckpoint(reached);
        if (next) {
            if (currentCheckpoint->reached) {
                StopBehavior(true);
                currentCheckpoint = next;
                StartBehavior();
            } else {
                checkpointDone = 1;
            }
        } else {
            finished = 1;
        }
    }
}

void CBuddyLeadBehavior::HandleCheckpointReachedByPlayer(int id) {
    Checkpoint_t *reached = g_checkpointMap[id];
    if (!reached->reached) {
        int mark = 1;
        CheckpointMap::iterator it(0);
        for (it = g_checkpointMap.begin(); it != g_checkpointMap.end(); ++it) {
            Checkpoint_t *checkpoint = (*it).second;
            if (checkpoint && checkpoint->order <= reached->order)
                checkpoint->reached = mark;
        }
    }
}

TriggerObject *CBuddyLeadBehavior::GetBuddyLeadCheckpointFromID(int id) { return g_checkpointMap[id]->trigger; }

static Checkpoint_t *GetNextCheckpoint(Checkpoint_t *checkpoint) {
    Checkpoint_t *next = 0;
    if (!checkpoint) {
        CheckpointMap::iterator it(0);
        for (it = g_checkpointMap.begin(); it != g_checkpointMap.end(); ++it) {
            Checkpoint_t *candidate = (*it).second;
            if (candidate && candidate->trigger->GetBool("IsFirst") == 1) {
                next = candidate;
                break;
            }
        }
    } else {
        next = g_checkpointMap[checkpoint->trigger->GetInt("NextNodeID")];
    }
    return next;
}
