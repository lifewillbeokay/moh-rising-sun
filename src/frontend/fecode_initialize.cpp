// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"
#include <string.h>

void FECodeListBox::Initialize(unsigned long newColumns, unsigned long newRows) {
    unsigned long oldColumns = numColumns, oldRows = numRows;
    FEListBoxCell *old = cells;
    numColumns = newColumns;
    numRows = newRows;
    cells = new ("source/FEngine/FECodeListBox.cpp:214") FEListBoxCell[newColumns * newRows];
    FEListBox::InitializeCell(cells, numColumns * numRows);
    SetTotalNumColumns(numColumns);
    SetTotalNumRows(numRows);
    if (listFlags & 1) {
        unsigned long keptColumns = numColumns < oldColumns ? numColumns : oldColumns;
        unsigned long keptRows = numRows < oldRows ? numRows : oldRows;
        if (old) {
            unsigned long row;
            for (row = 0; row < keptRows; ++row) {
                // The original copies the new row width here, even when growing.
                memcpy(cells + row * numColumns, old + row * oldColumns, numColumns * sizeof(FEListBoxCell));
                for (unsigned long column = keptColumns; column < oldColumns; ++column) {
                    FEListBoxCell &cell = old[row * oldColumns + column];
                    if (cell.type == 2) DeallocateString(cell.text);
                }
            }
            for (row = keptRows; row < oldRows; ++row) {
                for (unsigned long column = 0; column < oldColumns; ++column) {
                    FEListBoxCell &cell = old[row * oldColumns + column];
                    if (cell.type == 2) DeallocateString(cell.text);
                }
            }
            delete[] old;
        }
    }
    listFlags |= 1;
}

#endif
