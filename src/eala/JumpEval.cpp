// AI-assisted reconstruction of source/Character/Jump.cpp; see docs/Animation.md.
#include "profile.h"
#include "CharacterTasks.h"
namespace EALA {
namespace Character {

void Jump::EvalPose(float time, float *pose) {
    static float PERIOD = 1.0f / 30.0f;
    if (kept.IsValid())
        SetCorePose(kept->GetCorePose());
    if (!evaluated) {
        evaluated = 1;
        if (!blendFrames) {
            Pointer<Task> next = GetState()->EvalSubstate(target, parent.Get());
            if (next.IsValid()) {
                parent->Promote(next);
                next->EvalPose(time, pose);
                return;
            }
        } else {
            float blend = blendFrames * PERIOD;
            ParentIterator()->Set(target, blend);
        }
    }
    if (kept.IsValid())
        kept->EvalPose(time, pose);
}

} // namespace Character
} // namespace EALA
