// AI-assisted reconstruction from GR8E69; see docs/TriggerObject.md.
#include "mg_entries.h"
#include "SceneNode.h"
#include <math.h>
static inline void SubtractInto(CVector3 &out, const CVector3 &a, const CVector3 &b) {
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
}
int SearchForClosestMachineGun(ISceneNode *node, float maximumDistance) {
    CVector3 gunPosition, nodePosition, delta;
    float bestDistance = 10000.0f;
    int bestIndex = -1;
    for (unsigned int i = 0; i < 32; ++i) {
        TriggerObject *gun = g_pMGTriggerObject[i].positionTrigger;
        if (gun) {
            node->GetPosition(nodePosition);
            gun->GetPosition(gunPosition);
            SubtractInto(delta, nodePosition, gunPosition);
            float distance = sqrtf(delta.LengthSquared());
            if (!(distance >= bestDistance) && !(distance >= maximumDistance)) {
                bestDistance = distance;
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}
