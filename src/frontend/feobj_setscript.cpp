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

void FEObject::SetScript(unsigned long hash, bool restart) { SetScript(FindScript(hash), restart); }
void FEObject::SetScript(FEScript *script, bool restart) {
    if (restart || script != currentScript) {
        SetCurrentScript(script);
        currentScript->time = 0;
    }
}
