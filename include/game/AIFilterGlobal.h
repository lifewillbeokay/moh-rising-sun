#ifndef GAME_AI_FILTER_GLOBAL_H
#define GAME_AI_FILTER_GLOBAL_H

// Method-only interface to the original singleton; no object size or layout is
// inferred. FreePropertyMemory calls this before releasing the property data.
class CAIFilterGlobal {
public:
    void ShutdownSplinePathManager();
};
extern CAIFilterGlobal g_aigAIFilterGlobalObject;
#endif
