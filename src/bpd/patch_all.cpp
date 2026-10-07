#include "PropertyData.h"
#include "OffsetPtr.h"
#include "Endian.h"
#include "TriggerObject.h"
#include "Light.h"
#include "StringTable.h"
#include "FlexPropDatabase.h"
extern TriggerObject g_pTriggerObjects[];

int PatchUpAllPropertyData(BPDHeader *header) {
    // The header pointers for these records and volumes are already rebased.
    char *begin = (char *)header->animatedLights;
    unsigned int count = header->animatedLightCount;
    char *cursor = begin;
    TriggerObject *object = g_pTriggerObjects;
    if (header->field24) {
        offsetPtr(header->pointerTable, (int)header);
        for (int i = 0; i < header->field24; ++i) {
            void *&entry = header->pointerTable[i];
            ChangeEndian(entry);
            offsetPtr(entry, (int)header);
        }
    }
    if (header->lightPatterns) {
        offsetPtr(header->lightPatterns, (int)header);
        MOH_animatedLight_Struct *pattern = (MOH_animatedLight_Struct *)header->lightPatterns;
        for (int i = 0; i < header->lightPatternCount; ++i, ++pattern) {
            EndianSwap(*(xyzProperty_Struct *)pattern);
            EndianSwap(*pattern);
            offsetPtr(pattern->colours, (int)header);
            offsetPtr(pattern->times, (int)header);
            for (int j = 0; j < pattern->field72; ++j)
                ChangeEndian(pattern->times[j]);
        }
        CLight::Register(header->lightPatterns, header->lightPatternCount);
    }
    if (header->lightVolumes) {
        for (int i = 0; i < header->lightVolumeCount; ++i) {
            BPDLightVolume *volume = &header->lightVolumes[i];
            EndianSwap(*volume);
            if (volume->planeCount > 0) {
                offsetPtr(volume->planes, (int)header);
                for (int j = 0; j < header->lightVolumes[i].planeCount; ++j) {
                    PropPlane4 *plane = &header->lightVolumes[i].planes[j];
                    ChangeEndian(plane->x);
                    ChangeEndian(plane->y);
                    ChangeEndian(plane->z);
                    ChangeEndian(plane->d);
                }
            }
            if (volume->lightCount > 0) {
                offsetPtr(volume->lights, (int)header);
                for (int j = 0; j < header->lightVolumes[i].lightCount; ++j) {
                    BPDLight *light = &header->lightVolumes[i].lights[j];
                    ChangeEndian(light->intensity);
                    ChangeEndian(light->color[0]);
                    ChangeEndian(light->color[1]);
                    ChangeEndian(light->color[2]);
                    ChangeEndian(light->direction[0]);
                    ChangeEndian(light->direction[1]);
                    ChangeEndian(light->direction[2]);
                    ChangeEndian(light->position[0]);
                    ChangeEndian(light->position[1]);
                    ChangeEndian(light->position[2]);
                    ChangeEndian(light->type);
                }
            }
        }
    }
    for (unsigned int i = 0; i < count; ++i, ++object) {
        object->properties.legacy = (xyzProperty_Struct *)cursor;
        EndianSwap(*(xyzProperty_Struct *)cursor);
        switch (object->properties.legacy->field0c) {
        case 0:
            break;
        case 1:
        case 6:
            EndianSwap(*(MOH_core_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            break;
        case 2:
            EndianSwap(*(MOH_mechanic_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            PatchUpMechanic((MOH_mechanic_Struct *)object->properties.legacy);
            break;
        case 8:
            EndianSwap(*(MOH_mechanicEnvMod_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            PatchUpMechEnvMod((MOH_mechanicEnvMod_Struct *)object->properties.legacy);
            break;
        case 3:
            EndianSwap(*(MOH_enemy_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            PatchUpEnemy((MOH_enemy_Struct *)object->properties.legacy);
            break;
        case 13:
            EndianSwap(*(MOH_animatedLight_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            PatchUpAnimLight((MOH_animatedLight_Struct *)object->properties.legacy, (int)cursor);
            break;
        // Keep the original case bodies separate; their calls share a suffix.
        case 4:
            EndianSwap(*(MOH_core_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            break;
        case 5:
            EndianSwap(*(MOH_core_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            break;
        case 7:
            EndianSwap(*(MOH_core_Struct *)object->properties.legacy);
            PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            break;
        default:
            if (object->properties.legacy->field0c <= 14) {
                EndianSwap(*(MOH_core_Struct *)object->properties.legacy);
                PatchUpCore((MOH_core_Struct *)object->properties.legacy);
            } else {
                DebugMsg("Unknown property type\n");
            }
            break;
        }
        cursor += object->properties.legacy->field04;
    }
    offsetPtr(header->objects, (int)header);
    offsetPtr(header->classLayouts, (int)header);
    offsetPtr(header->strings, (int)header);
    g_pStringTable = StringTable::Create((char *)header->strings);
    FlexPropDatabase::LoadClasses(header->classLayouts, header->classLayoutCount);
    FlexPropDatabase::LoadProperties(header, header->objects, header->objectCount);
    return cursor - begin;
}
