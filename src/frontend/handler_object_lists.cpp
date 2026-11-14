// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "FEngObjects.h"
#include <stdio.h>
extern "C" int strcasecmp(const char *, const char *);
int PackageHandler::SetObjectListIndex(char *prefix, unsigned int index) {
    char name[64]; sprintf(name, "%s%d", prefix, index);
    if (!package->FindObjectByName(name)) return -1;
    for (int i = 1; ; ++i) {
        sprintf(name, "%s%d", prefix, i);
        if (SetScript(package, name, "Hide")) break;
    }
    sprintf(name, "%s%d", prefix, index); SetScript(package, name, "Show");
    return 0;
}
int PackageHandler::SetNextObjectListIndex(char *prefix) {
    char name[64];
    for (int i = 1; ; ++i) {
        sprintf(name, "%s%d", prefix, i);
        FEObject *object = package->FindObjectByName(name);
        if (object && object->currentScript && !strcasecmp(object->currentScript->name, "Show")) {
            SetScript(package, name, "Hide");
            sprintf(name, "%s%d", prefix, i + 1);
            if (SetScript(package, name, "Show")) {
                sprintf(name, "%s%d", prefix, 1); SetScript(package, name, "Show");
            }
            return 0;
        }
    }
}
int PackageHandler::SetPrevObjectListIndex(char *prefix) {
    char name[64];
    for (int i = 1; ; ++i) {
        sprintf(name, "%s%d", prefix, i);
        FEObject *object = package->FindObjectByName(name);
        if (object && object->currentScript && !strcasecmp(object->currentScript->name, "Show")) {
            SetScript(package, name, "Hide"); sprintf(name, "%s%d", prefix, i - 1);
            if (SetScript(package, name, "Show")) {
                int last = 1;
                do { sprintf(name, "%s%d", prefix, last++); } while (package->FindObjectByName(name));
                sprintf(name, "%s%d", prefix, last - 2); SetScript(package, name, "Show");
            }
            return 0;
        }
    }
}
int PackageHandler::GetObjectListIndex(char *prefix) {
    char name[64];
    for (int i = 1; ; ++i) {
        sprintf(name, "%s%d", prefix, i);
        FEObject *object = package->FindObjectByName(name);
        if (object && object->currentScript && !strcasecmp(object->currentScript->name, "Show")) return i;
    }
}
#endif
