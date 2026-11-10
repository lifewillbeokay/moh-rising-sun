// AI-assisted reconstruction from GR8E69. Fragments of the original bot object source: the
// original file name is not known. Method names come from the symbol table; member names
// are inferred, see docs/Behaviors.md.
#include "AIBot.h"

int CBotObject::BS_Chase() {
    LookAlongPath();
    BS_Goto();
    return 1;
}
