// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include "FEListIndex.h"
#include "ShellManager.h"

void FECodeListBox::FillAllCells() {
    if (!totalColumns || !totalRows || !numRows || !numColumns) return;
    long firstColumn = currentColumn, row = currentRow;
    unsigned long rows = numRows < totalRows ? numRows : totalRows;
    unsigned long columns = numColumns < totalColumns ? numColumns : totalColumns;
    typedef void (*FillCallback)(void *, FECodeListBox *, unsigned long, unsigned long);
    if (unknown_bc) {
        for (unsigned long y = 0; y < rows; ++y) {
            long column = firstColumn;
            for (unsigned long x = 0; x < columns; ++x) {
                ((FillCallback)unknown_bc)((void *)unknown_c0, this, column, row);
                column = FEWrapIndex(column + 1, totalColumns);
            }
            row = FEWrapIndex(row + 1, totalRows);
        }
    } else if (game) {
        for (unsigned long y = 0; y < rows; ++y) {
            long column = firstColumn;
            for (unsigned long x = 0; x < columns; ++x) {
                game->SetCellData(this, column, row);
                column = FEWrapIndex(column + 1, totalColumns);
            }
            row = FEWrapIndex(row + 1, totalRows);
        }
    }
}

#endif
