// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

FECodeListBox::FECodeListBox() : game(0), numColumns(0), numRows(0), listFlags(2), totalColumns(0), totalRows(0), currentColumn(0), currentRow(0), targetColumn(0), targetRow(0), unknown_80(0.0f), unknown_84(0.0f), cells(0), unknown_8c(0), unknown_90(0), stringsInUse(0), strings(0), stringBlock(0), blink(1.0f), blinkRate(-0.00138888892f), selectColor(0xffffffff), selectCallback(mpDefaultCallback), unknown_bc(0), unknown_c0(0) {
    type = 6;
}
