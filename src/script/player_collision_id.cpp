// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "SceneObjects.h"

EClsnId GetCollisionIDFromScriptCollisionType(EPlayerCollisionType type) {
    int id = -1;
    switch (type) {
    case 0:
        id = 1;
        break;
    case 1:
        break;
    case 2:
        id = 21;
        break;
    }
    return (EClsnId)id;
}
