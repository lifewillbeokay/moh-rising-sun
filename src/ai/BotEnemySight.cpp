// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

int CBotObject::EnemyJustGoneOutOfSight() {
    static int OUT_OF_SIGHT_DELAY = 30;
    return enemyOutOfSightFrames == OUT_OF_SIGHT_DELAY;
}
