// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"

void FECodeListBox::DefaultSelectCallback(FECodeListBox *list) {
    FEColor color = list->selectColor;
    color.a = (int)(list->blink * 255.0f);
    list->selectColor = color;
}

#endif
