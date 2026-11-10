// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include "ContainerTypes.h"
PathFindingNetwork::PathFindingNetwork(const PathFindingNetwork& other) : id(other.id), links(other.links) {}
