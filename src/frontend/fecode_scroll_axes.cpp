// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEngElements.h"

void FECodeListBox::ScrollSelection(long column, long row) {
    ScrollSelection(column, currentColumn, targetColumn, totalColumns, numColumns, true);
    ScrollSelection(row, currentRow, targetRow, totalRows, numRows, false);
}
