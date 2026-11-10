// FEScript.cpp: scripts, their tracks and their pooled allocation.
#include "FEScript.h"
#include "FEHash.h"
#include <string.h>
typedef FEPoolNode<FEScript, 32> FEScriptPoolNode;
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FEScript::Init() {
    name = 0;
    hash = 0;
    unknown_c = 0;
    unknown_14 = 0;
    unknown_18 = 0;
    trackCount = 0;
    tracks = 0;
}
FEScript::~FEScript() {
    if (tracks) delete[] tracks;
    int zero = 0;
    tracks = (FEKeyTrack *)zero;
    if (name) delete[] name;
    name = (char *)zero;
}
