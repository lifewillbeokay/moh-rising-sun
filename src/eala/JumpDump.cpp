// AI-assisted reconstruction of source/Character/Jump.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

void Jump::Dump(unsigned int depth) {
    DumpPrefix(depth);
    DebugMsg("Jump (to %d)\n", target);
    if (kept.IsValid()) {
        DumpPrefix(depth);
        DebugMsg("KEEPER:\n");
        kept->Dump(depth + 1);
    }
}

} // namespace Character
} // namespace EALA
