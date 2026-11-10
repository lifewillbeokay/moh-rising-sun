// FEListBox: rows and columns of cells.
#include "FEListBox.h"
#include <string.h>

void FEListBox::CleanupColumns() {
    if (numColumns) {
        if (columns) delete[] columns;
        columns = 0;
        numColumns = 0;
    }
}
void FEListBox::CleanupRows() {
    if (numRows) {
        if (rows) delete[] rows;
        rows = 0;
        numRows = 0;
    }
}
void FEListBox::CleanupCells() {
    unsigned long count = numRows * numColumns;
    if (count) {
        for (unsigned long i = 0; i < count; i++) {
            if (cells[i].type == 2 && cells[i].text) {
                delete[] cells[i].text;
                cells[i].text = 0;
            }
        }
        if (cells) delete[] cells;
        cells = 0;
    }
}
void FEListBox::RecalculateCummulative() {
    FEListEntryData *entry = columns;
    float total = 0.0f;
    for (unsigned long i = 0; i < numColumns; i++) {
        entry->cumulative = total;
        total += entry->size;
        entry++;
    }
    entry = rows;
    total = 0.0f;
    for (unsigned long i = 0; i < numRows; i++) {
        entry->cumulative = total;
        total += entry->size;
        entry++;
    }
}
void FEListBox::CompleteScroll() {
    listFlags &= ~98;
    scrollX = targetX;
    scrollY = targetY;
    if (currentColumn == 0) {
        listFlags &= ~8;
        scrollX = 0.0f;
    }
    if (currentRow == 0) {
        listFlags &= ~16;
        scrollY = 0.0f;
    }
}
FEListBoxCell *FEListBox::GetPCellData(unsigned long column, unsigned long row) {
    return &cells[row * numColumns + column];
}
