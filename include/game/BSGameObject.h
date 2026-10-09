// AI-assisted reconstruction from GR8E69; see docs/Script.md.
// Observed prefix only, not a complete allocation. Member names are descriptive;
// vtable entries establish the inherited subject interface and new slot order.
#ifndef GAME_BS_GAME_OBJECT_H
#define GAME_BS_GAME_OBJECT_H
#pragma interface
#include "ObserverTypes.h"
class ISceneNode;
class BSGO_Basic : public ISubject {
public:
    unsigned char unknown08[4];
    BSGO_Basic *next;
    BSGO_Basic *previous;
    virtual ~BSGO_Basic();
    void Destroy();
    // Pointer ABI placeholders: concrete script/proximity data types are unknown.
    virtual void *GetScriptData();
    virtual ISceneNode *GetSceneNode();
    virtual void *GetProximityData();
};
void AddObjectToList(BSGO_Basic *, BSGO_Basic **);
void RemoveObjectFromList(BSGO_Basic *, BSGO_Basic **);
typedef char BSGameObjectPrefixCheck[sizeof(BSGO_Basic) == 0x14 ? 1 : -1];
#endif
