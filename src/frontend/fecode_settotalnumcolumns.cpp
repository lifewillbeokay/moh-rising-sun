// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FECodeListBox::SetTotalNumColumns(unsigned long total) {
    totalColumns = total;
    currentColumn = CalculateCurrentFromTarget(targetColumn, total, numColumns);
}
void FECodeListBox::SetTotalNumRows(unsigned long total) {
    totalRows = total;
    currentRow = CalculateCurrentFromTarget(targetRow, total, numRows);
}
