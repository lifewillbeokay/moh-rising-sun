#include "PropertyData.h"
#include "loaders_internal.h"

void LoadPropertyBPD(char *filename, bool reset) {
    if (!reset) {
        int size;
        g_pBPDHeader = (BPDHeader *)TLT_LoadFileFromLevelBigFile(filename, &size);
        EndianSwap(*g_pBPDHeader);
        g_pNumTriggers = g_pBPDHeader->animatedLightCount;
        g_pBPDHeader->animatedLights = (void *)((int)g_pBPDHeader + (int)g_pBPDHeader->animatedLights);
        g_pBPDHeader->paths = (BPDPolyPath *)((int)g_pBPDHeader + (int)g_pBPDHeader->paths);
        g_pBPDHeader->lightVolumes = (BPDLightVolume *)((int)g_pBPDHeader + (int)g_pBPDHeader->lightVolumes);
        PatchUpAllPropertyData(g_pBPDHeader);
        g_aigAIFilterGlobalObject.CreateSplinePathManager(g_pBPDHeader, g_pBPDHeader->paths, g_pBPDHeader->pathCount);
        if (g_pBPDHeader->field00 == 13)
            AIPathFinding::ImportAreaPathNodeNetwork(g_pBPDHeader);
        g_pRawPropertyData = g_pBPDHeader->animatedLights;
        g_pLightVolumes = g_pBPDHeader->lightVolumes;
    } else {
        g_pNumTriggers = g_pBPDHeader->animatedLightCount;
        g_aigAIFilterGlobalObject.ResetSplinePathManager(g_pBPDHeader, g_pBPDHeader->paths, g_pBPDHeader->pathCount);
    }
    InitAllTriggers();
    ClearMGTriggerObjects();
    FindAllMGPointsInBPDFile();
    CBuddyLeadBehavior::CacheBuddyLeadTriggers();
    CMountedMachineGunBehavior::DeregisterMMGTriggers();
    CMountedMachineGunBehavior::RegisterMMGTriggers();
}
