// AI-assisted reconstruction of CAITargeting's script-registered location targets; see docs/Behaviors.md.
#include "profile.h"
#include "AITargeting.h"
#include "ScriptRuntime.h"
#include <map>
void BSObjectTriggerEvent(BSObject *, unsigned short, void *, BSObject *, bool);
typedef _STL::map<int, _LocationTargetInfo> TargetMap;
extern TargetMap g_registered_targets;

void CAITargeting::ResetLocationTargets() { g_registered_targets.clear(); }

void CAITargeting::RegisterLocationTarget(int id, bool enabled, ETeam team, BSObject *object) {
    _LocationTargetInfo info;
    info.enabled = enabled;
    info.team = team;
    info.object = object;
    if (id == -1)
        g_registered_targets[(int)object] = info;
    else
        g_registered_targets[id] = info;
}

void CAITargeting::DeRegisterLocationTarget(int id) {
    TargetMap::iterator found(0);
    found = g_registered_targets.find(id);
    if (found != g_registered_targets.end()) {
        g_registered_targets.erase(found);
        for (CAIObject *object = CAIObject::GetGlobalListStart(); object; object = object->nextGlobal) {
            CAITargeting *targeting = &object->targeting;
            if (targeting && targeting->IsNonAITarget()) {
                CAITarget target = targeting->GetTarget();
                if ((int)target.scriptObject == id) {
                    BSObjectTriggerEvent(object->scriptObject, 65, 0, 0, true);
                    targeting->SetNonAITarget(0);
                }
            }
        }
    }
}

_LocationTargetInfo CAITargeting::GetLocationTarget(int id) {
    _LocationTargetInfo missing;
    missing.object = 0;
    TargetMap::iterator found(0);
    found = g_registered_targets.find(id);
    if (found == g_registered_targets.end())
        return missing;
    return (*found).second;
}
