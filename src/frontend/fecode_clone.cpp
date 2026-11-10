// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

FEObject *FECodeListBox::Clone(bool deep) { return new ("source/FEngine/FECodeListBox.cpp:278") FECodeListBox(*this, deep); }
