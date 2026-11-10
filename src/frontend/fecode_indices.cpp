// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include "FEListIndex.h"

long FECodeListBox::GetRealColumn(long column) const {
    long total = totalColumns, visible = numColumns;
    if (!total) return -1;
    if (column >= total) column %= total;
    column -= currentColumn;
    if (column < 0) column += total;
    return FEWrapIndex(column, visible);
}

long FECodeListBox::GetRealRow(long row) const {
    long total = totalRows, visible = numRows;
    if (!total) return -1;
    if (row >= total) row %= total;
    row -= currentRow;
    if (row < 0) row += total;
    return FEWrapIndex(row, visible);
}

#endif
