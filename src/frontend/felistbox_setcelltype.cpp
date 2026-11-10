// FEListBox: rows and columns of cells.
#include "FEListBox.h"
#include <string.h>

void FEListBox::SetCellType(unsigned long type) {
    FEListBoxCell *cell = &cells[currentRow * numColumns + currentColumn];
    if (cell->type != type) {
        if (cell->type == 2 && cell->text) {
            delete[] cell->text;
            cell->text = 0;
        }
        cell->type = type;
    }
}
void FEListBox::SetCellString(const short *text) {
    FEListBoxCell *cell = &cells[currentRow * numColumns + currentColumn];
    if (cell->text) {
        delete[] cell->text;
        cell->text = 0;
    }
    if (text) {
        long size = (GetStringLength(text) + 1) * 2;
        void *copy = DWI_alloc("source/FEngine/FEListBox.cpp:528", size, 256);
        cell->text = (short *)copy;
        memcpy(copy, (const void *)text, size);
    }
}
void FEListBox::IncrementCellByColumn() {
    if (++currentColumn >= numColumns) {
        currentColumn = 0;
        if (++currentRow >= numRows) currentRow = 0;
    }
}
