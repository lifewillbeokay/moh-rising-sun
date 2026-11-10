// FEMsgTargetList: a growable array of message targets; FEPackage's lookup of the list for a message.
#include "FEngLists.h"
#include <string.h>
extern "C" void *DWI_alloc(const char *, int, int);
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FEMsgTargetList::Allocate(unsigned long size) {
    if (size == 0) {
        if (targets) delete[] targets;
        targets = (FEObject **)size;
        count = size;
        allocated = size;
    } else {
        if (size != allocated) {
            void *buffer = new ("source/FEngine/FEMsgTargetList.cpp:23") FEObject *[size];
            if (size < allocated) memcpy(buffer, targets, size * sizeof(FEObject *));
            else memcpy(buffer, targets, allocated * sizeof(FEObject *));
            if (targets) delete[] targets;
            targets = (FEObject **)buffer;
        }
    }
    allocated = size;
}
void FEMsgTargetList::AppendTarget(FEObject *target) {
    if (count == allocated) Allocate(count + 1);
    targets[count++] = target;
}
FEMsgTargetList *FEPackage::GetMessageTargets(unsigned long message) {
    for (unsigned long i = 0; i < messageTargetCount; i++) {
        if (messageTargets[i].message == message) return &messageTargets[i];
    }
    return 0;
}
