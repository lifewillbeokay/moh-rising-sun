// FEString: copying and labels.
#include "FEngElements.h"
#include "FEHash.h"
extern "C" void *DWI_alloc(const char *, int, int);
#include <string.h>

FEString::FEString(const FEString &other, bool deep) : FEObject(other, deep), label(0), text(other.text) {
    unknown_68 = other.unknown_68;
    unknown_6c = other.unknown_6c;
    unknown_70 = other.unknown_70;
    SetLabel(other.label);
    text.SetLength(other.text.capacity);
}
void FEString::SetLabel(const char *newLabel) {
    if (label) delete[] label;
    label = 0;
    if (newLabel) {
        label = new ("source/FEngine/FEString.cpp:29") char[strlen(newLabel) + 1];
        strcpy(label, newLabel);
    }
    labelHash = FEHashUpper(label);
    flags |= 0x400000;
    if (pLabelCallback) pLabelCallback->LabelRead(this);
}
