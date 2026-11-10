// FEngine.cpp: queued messages and package commands.
#include "FEngObjects.h"
#include "FEngStrings.h"
#include <string.h>
class FEJoyPad { public: FEJoyPad(); unsigned char state[136]; };
class FEListBox { public: void ScrollSelection(long, long); };
class FECodeListBox { public: void ScrollSelection(long, long); };
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

int FEngine::ProcessListBoxResponses(FEObject *object, FEPackage *, unsigned long message) {
    switch (message) {
    case 0xe10c4af9:
        ((FEListBox *)object)->ScrollSelection(-1, 0);
        return 1;
    case 0x030471ac:
        ((FEListBox *)object)->ScrollSelection(1, 0);
        return 1;
    case 0xfb814f13:
        ((FEListBox *)object)->ScrollSelection(0, -1);
        return 1;
    case 0xe10814a6:
        ((FEListBox *)object)->ScrollSelection(0, 1);
        return 1;
    }
    return 0;
}
int FEngine::ProcessCodeListBoxResponses(FEObject *object, FEPackage *, unsigned long message) {
    switch (message) {
    case 0xe10c4af9:
        ((FECodeListBox *)object)->ScrollSelection(-1, 0);
        return 1;
    case 0x030471ac:
        ((FECodeListBox *)object)->ScrollSelection(1, 0);
        return 1;
    case 0xfb814f13:
        ((FECodeListBox *)object)->ScrollSelection(0, -1);
        return 1;
    case 0xe10814a6:
        ((FECodeListBox *)object)->ScrollSelection(0, 1);
        return 1;
    }
    return 0;
}
void FEngine::ProcessObjectMessage(FEObject *object, FEPackage *package, unsigned long message, unsigned long mask) {
    if (object->type == 4 && ProcessListBoxResponses(object, package, message)) return;
    if (object->type == 6 && ProcessCodeListBoxResponses(object, package, message)) return;
    FEMessageResponse *response = object->FindResponse(message);
    if (response) ProcessResponses(response, object, package, mask);
}
void FEngine::ProcessGlobalMessage(FEPackage *package, unsigned long message, unsigned long mask) {
    FEMessageResponse *response = package->FindResponse(message);
    if (response) ProcessResponses(response, 0, package, mask);
}
