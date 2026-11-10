// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include "FEListIndex.h"
#include "ShellManager.h"
#include <string.h>

static inline void FillCell(FECodeListBox *list, unsigned long column, unsigned long row) {
    typedef void (*FillCallback)(void *, FECodeListBox *, unsigned long, unsigned long);
    if (list->unknown_bc)
        ((FillCallback)list->unknown_bc)((void *)list->unknown_c0, list, column, row);
    else
        list->game->SetCellData(list, column, row);
}

int FECodeListBox::ScrollSelection(long step, unsigned long &current, unsigned long &target,
                                  unsigned long total, unsigned long visible, bool horizontal) {
    if (!step || !CheckMovement(step, current, target, total, visible)) return 0;
    if (!MakeMove(step, current, target, total, visible)) return 0;
    if (!total) return 1;
    if (!unknown_bc && !game) return 1;
    if (horizontal) {
        if (step > 0) {
            unsigned long width = numColumns;
            long column = FEWrapIndex(currentColumn + width - 1, totalColumns);
            for (unsigned long row = 0; row < numRows; ++row) {
                short *recycled = cells[row * numColumns].text;
                unsigned long x;
                for (x = 0; x < width - 1; ++x)
                    memcpy(&cells[row * numColumns + x], &cells[row * numColumns + x + 1], sizeof(FEListBoxCell));
                cells[row * numColumns + x].text = recycled;
                FillCell(this, column, FEWrapIndex(currentRow + row, totalRows));
            }
        } else {
            for (unsigned long row = 0; row < numRows; ++row) {
                short *recycled = cells[row * numColumns + numColumns - 1].text;
                for (long x = numColumns - 1; x; --x)
                    memcpy(&cells[row * numColumns + x], &cells[row * numColumns + x - 1], sizeof(FEListBoxCell));
                cells[row * numColumns].text = recycled;
                FillCell(this, currentColumn, FEWrapIndex(currentRow + row, totalRows));
            }
        }
    } else if (step > 0) {
        unsigned long height = numRows;
        long row = FEWrapIndex(currentRow + height - 1, totalRows);
        for (unsigned long column = 0; column < numColumns; ++column) {
            short *recycled = cells[column].text;
            unsigned long y;
            for (y = 0; y < height - 1; ++y)
                memcpy(&cells[y * numColumns + column], &cells[(y + 1) * numColumns + column], sizeof(FEListBoxCell));
            cells[y * numColumns + column].text = recycled;
            FillCell(this, FEWrapIndex(currentColumn + column, totalColumns), row);
        }
    } else {
        for (unsigned long column = 0; column < numColumns; ++column) {
            short *recycled = cells[(numRows - 1) * numColumns + column].text;
            for (long y = numRows - 1; y; --y)
                memcpy(&cells[y * numColumns + column], &cells[(y - 1) * numColumns + column], sizeof(FEListBoxCell));
            cells[column].text = recycled;
            // Unlike the positive path, the original passes the same column each time.
            FillCell(this, currentColumn, FEWrapIndex(currentRow, totalRows));
        }
    }
    return 1;
}

#endif
