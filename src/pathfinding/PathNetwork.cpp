// AI-assisted reconstruction of the path network links; see docs/Paths.md. Names come from the symbol table.
#include "profile.h"
#include <list>
#include <map>
#include "ContainerTypes.h"
#include "CVector3.h"
#include "TriggerObject.h"
#include <string.h>
extern _STL::map<unsigned char, PathFindingNetwork> g_networks;
typedef _STL::list<PathFindingNetworkLink> LinkList;

void PathFindingNetwork::RegisterLinkTo(TriggerObject *trigger, int network, int point, int link) {
    bool found = false;
    LinkList::iterator it;
    for (it = links.begin(); it != links.end(); ++it) {
        PathFindingNetworkLink &entry = *it;
        if (entry.network == network && entry.pointIndex == point && entry.linkIndex == link) {
            entry.fromTrigger = trigger;
            found = true;
            break;
        }
    }
    if (!found) {
        PathFindingNetworkLink entry;
        entry.linkIndex = link;
        entry.pointIndex = point;
        entry.network = network;
        entry.fromTrigger = trigger;
        links.push_back(entry);
    }
}

void PathFindingNetwork::RegisterLinkFrom(TriggerObject *trigger, int point, int network, int link) {
    bool found = false;
    LinkList::iterator it;
    for (it = links.begin(); it != links.end(); ++it) {
        PathFindingNetworkLink &entry = *it;
        if (entry.network == network && entry.pointIndex == point && entry.linkIndex == link) {
            entry.toTrigger = trigger;
            found = true;
            break;
        }
    }
    if (!found) {
        PathFindingNetworkLink entry;
        entry.linkIndex = link;
        entry.pointIndex = point;
        entry.network = network;
        entry.toTrigger = trigger;
        links.push_back(entry);
    }
}

int PathFindingNetwork::GetLinkGoingTo(const CVector3 &position, unsigned char network, unsigned char *visited,
                                       TriggerObject **first, TriggerObject **second) {
    CVector3 point;
    int found = 0;
    float closest = 0.0f;
    LinkList::iterator it;
    for (it = links.begin(); it != links.end(); ++it) {
        if ((*it).network == network) {
            if (!first || !second)
                goto linked;
            (*it).toTrigger->GetPosition(point);
            float distance = point.DistanceSquaredXY(position);
            if (!found || !(distance >= closest)) {
                closest = distance;
                found = 1;
                *first = (*it).toTrigger;
                *second = (*it).fromTrigger;
            }
        }
    }
    if (found) {
    linked:
        return 1;
    }
    visited[id] = 1;
    for (it = links.begin(); it != links.end(); ++it) {
        if (!visited[(*it).network]) {
            _STL::map<unsigned char, PathFindingNetwork>::iterator next = g_networks.find((*it).network);
            if ((*next).second.GetLinkGoingTo(position, network, visited, 0, 0)) {
                if (!first || !second)
                    goto linked;
                (*it).toTrigger->GetPosition(point);
                float distance = point.DistanceSquaredXY(position);
                if (!found || !(distance <= closest)) {
                    closest = distance;
                    found = 1;
                    *first = (*it).toTrigger;
                    *second = (*it).fromTrigger;
                }
            }
        }
    }
    return found;
}

int GetNextNetworkLink(const CVector3 &position, unsigned char from, unsigned char to, TriggerObject **first,
                       TriggerObject **second) {
    unsigned char visited[256];
    memset(visited, 0, 256);
    _STL::map<unsigned char, PathFindingNetwork>::iterator found(0);
    found = g_networks.find(from);
    if (found == g_networks.end())
        return 0;
    return (*found).second.GetLinkGoingTo(position, to, visited, first, second);
}

int NetworksAreLinked(unsigned char from, unsigned char to) {
    unsigned char visited[256];
    memset(visited, 0, 256);
    _STL::map<unsigned char, PathFindingNetwork>::iterator found(0);
    found = g_networks.find(from);
    if (found == g_networks.end())
        return 0;
    CVector3 origin;
    origin.Zero();
    return (*found).second.GetLinkGoingTo(origin, to, visited, 0, 0);
}
