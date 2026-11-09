// AI-assisted GR8E69 reconstruction; Frontline reference and differences in
// docs/research/broad-source-sweep.md.
#include "PropertyReaders.h"

int LoadPropertyData(xyzProperty_Struct *object, int field) {
    switch (object->field0c) {
    case 1:
        return _LoadPropertyData(reinterpret_cast<MOH_core_Struct *>(object), field);
    case 2:
        return _LoadPropertyData(reinterpret_cast<MOH_mechanic_Struct *>(object), field);
    case 3:
        return _LoadPropertyData(reinterpret_cast<MOH_enemy_Struct *>(object), field);
    case 4:
        return _LoadPropertyData(reinterpret_cast<MOH_powerup_Struct *>(object), field);
    case 5:
        return _LoadPropertyData(reinterpret_cast<MOH_wayPoint_Struct *>(object), field);
    case 6:
        return _LoadPropertyData(reinterpret_cast<MOH_pathPoint_Struct *>(object), field);
    case 7:
        return _LoadPropertyData(reinterpret_cast<MOH_envMod_Struct *>(object), field);
    case 8:
        return _LoadPropertyData(reinterpret_cast<MOH_mechanicEnvMod_Struct *>(object), field);
    case 9:
        return _LoadPropertyData(reinterpret_cast<MOH_projectileGenerator_Struct *>(object), field);
    case 10:
        return _LoadPropertyData(reinterpret_cast<MOH_projectileGeneratorTarget_Struct *>(object), field);
    case 11:
        return _LoadPropertyData(reinterpret_cast<MOH_cameraShaker_Struct *>(object), field);
    case 12:
        return _LoadPropertyData(reinterpret_cast<MOH_playerPathController_Struct *>(object), field);
    case 13:
        return _LoadPropertyData(reinterpret_cast<MOH_animatedLight_Struct *>(object), field);
    case 14:
        return _LoadPropertyData(reinterpret_cast<MOH_fogParams_Struct *>(object), field);
    }
    return 0;
}
