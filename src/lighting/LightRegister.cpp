#include "Light.h"
extern CAnimLightManager g_AnimLightManager;
void CLight::Register(void *patterns, int count) {
    g_AnimLightManager.patterns = patterns;
    g_AnimLightManager.patternCount = count;
}
