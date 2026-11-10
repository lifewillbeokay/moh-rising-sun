// FEPackageList: packages sorted by priority.
#include "FEngLists.h"
extern "C" int strcasecmp(const char *, const char *);

void FEPackageList::AddPackage(FEPackage *package) {
    FEPackage *after = (FEPackage *)tail;
    if (after && after->priority > package->priority) {
        do {
            after = (FEPackage *)after->prev;
            if (!after) break;
        } while (after->priority > package->priority);
    }
    AddNode(after, package);
}
FEPackage *FEPackageList::FindPackage(const char *name, unsigned char group) const {
    int mask = 1 << group;
    unsigned long hash = FEHashUpper(name);
    for (FEPackage *p = (FEPackage *)head; p; p = (FEPackage *)p->next) {
        if (p->hash == hash && mask == p->controlMask && strcasecmp(p->name, name) == 0) return p;
    }
    return 0;
}
FEPackage *FEPackageList::FindPackage(unsigned char group) const {
    for (FEPackage *p = (FEPackage *)head; p; p = (FEPackage *)p->next) {
        if (group == p->groupId) return p;
    }
    return 0;
}
int FEPackageList::RemovePackage(FEPackage *package) {
    for (FEMinNode *node = head; node; node = node->next) {
        if (node == package) {
            RemNode(node);
            return 1;
        }
    }
    return 0;
}
void FEPackageList::ReplaceParentLinks(const FEPackage *from, const FEPackage *to) {
    for (FEPackage *p = (FEPackage *)head; p; p = (FEPackage *)p->next) {
        if (p->parent == from) p->parent = to;
    }
}
