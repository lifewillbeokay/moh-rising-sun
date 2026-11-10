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

void FEObject::SetCurrentScript(FEScript *script) {
    currentScript = script;
    if (script) SetupMoveToTracks();
}
FEMessageResponse *FEObject::FindResponse(unsigned long message) const {
    FEMessageResponse *response = (FEMessageResponse *)responseList.head;
    while (response && response->message != message) response = (FEMessageResponse *)response->next;
    return response;
}
