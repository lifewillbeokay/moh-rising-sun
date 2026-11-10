// FEObject's small methods; see docs/Frontend.md.
#include "FEngObjects.h"
class FEMultiPool {
  public:
    unsigned char *Alloc(unsigned long);
    void Free(unsigned char *);
};
extern FEMultiPool ObjDataPool;
class FEMessageResponse : public FEMinNode {
  public:
    unsigned long message; // +0xc
};

FEScript *FEObject::FindScript(unsigned long hash) const {
    FEScript *script = (FEScript *)scriptList.head;
    while (script && script->hash != hash) script = (FEScript *)script->next;
    return script;
}
