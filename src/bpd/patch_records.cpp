#include "PropertyData.h"
#include "OffsetPtr.h"
#include "Endian.h"

void PatchUpCore(MOH_core_Struct *record) {
    if (record->field58) {
        record->field58 = (unsigned long *)((int)record + (int)record->field58);
        EndianSwapList(record->field58);
    }
    if (record->field5c) {
        record->field5c = (unsigned long *)((int)record + (int)record->field5c);
        EndianSwapList(record->field5c);
    }
    if (record->field60) {
        record->field60 = (unsigned long *)((int)record + (int)record->field60);
        EndianSwapList(record->field60);
    }
    if (record->field64) {
        record->field64 = (unsigned long *)((int)record + (int)record->field64);
        EndianSwapList(record->field64);
    }
    if (record->field44) {
        record->field44 = (unsigned long *)((int)record + (int)record->field44);
        EndianSwapList(record->field44);
    }
    if (record->field68) {
        record->field68 = (unsigned long *)((int)record + (int)record->field68);
        EndianSwapList(record->field68);
    }
    if (record->field40)
        record->field40 = (char *)((int)record + (int)record->field40);
    if (record->field4c)
        record->field4c = (char *)((int)record + (int)record->field4c);
    if (record->field54)
        record->field54 = (char *)((int)record + (int)record->field54);
    if (record->field6c) {
        record->field6c = (unsigned long *)((int)record + (int)record->field6c);
        EndianSwapList(record->field6c);
    }
}
void PatchUpMechanic(MOH_mechanic_Struct *) {}
void PatchUpMechEnvMod(MOH_mechanicEnvMod_Struct *) {}
void PatchUpEnemy(MOH_enemy_Struct *) {}
void PatchUpAnimLight(MOH_animatedLight_Struct *record, int base) {
    offsetPtr(record->colours, base);
    offsetPtr(record->times, base);
    for (int i = 0; i < record->field72; ++i)
        ChangeEndian(record->times[i]);
}
