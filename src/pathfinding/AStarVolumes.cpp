// AI-assisted reconstruction of the path finder's script-registered volumes; see docs/Paths.md.
// Tags come from the symbol table; AStarVolume's member names are inferred.
#include "profile.h"
#include <list>
#include <map>
#include <algorithm>
#include "AISplinePath.h"
#include "ContainerTypes.h"
#include "TriggerObject.h"
struct AStarVolume {
    unsigned char network;
    TriggerObject *trigger;
    bool operator==(const AStarVolume &other) const;
};
extern _STL::map<unsigned char, PathFindingNetwork> g_networks;
extern _STL::list<AStarVolume> g_astar_volumes;

void CAIAreaPathFinding::ResetAStarVolumes() {
    g_networks.clear();
    g_astar_volumes.clear();
}

void CAIAreaPathFinding::RegisterAStarVolume(TriggerObject *trigger, int network) {
    AStarVolume volume;
    volume.network = network;
    volume.trigger = trigger;
    _STL::list<AStarVolume>::iterator found;
    found = _STL::find(g_astar_volumes.begin(), g_astar_volumes.end(), volume);
    if (found == g_astar_volumes.end())
        g_astar_volumes.push_back(volume);
}

int CAIAreaPathFinding::WhichNetwork(const CVector3 &point) {
    _STL::list<AStarVolume>::iterator it;
    for (it = g_astar_volumes.begin(); it != g_astar_volumes.end(); ++it) {
        if (IsPointInTrigger(point, *(*it).trigger))
            return (*it).network;
    }
    return 0;
}
