// FEKeyTrack's keys and their pooled allocation.
#include "FEScript.h"
typedef FEPoolNode<FEKeyNode, 256> FEKeyNodePoolNode;
class FEColor {
  public:
    FEColor &operator=(const FEColor &);
    FEColor operator-(const FEColor &) const;
    int b, g, r, a;
};

FEKeyNode *FEKeyTrack::CreateKeyAt(long time) {
    FEKeyNode *key;
    for (key = (FEKeyNode *)keys.GetHead(); key; key = (FEKeyNode *)key->next) {
        if (key->time >= time) break;
    }
    if (!key || key->time != time) {
        key = new FEKeyNode;
        key->time = time;
        InsertKey(key);
    }
    return key;
}
