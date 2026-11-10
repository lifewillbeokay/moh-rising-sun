// FECodeListBox: the list box whose cells the game fills through a callback.
#include "FEngElements.h"
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

void FECodeListBox::SetCellColor(unsigned long column, unsigned long row, unsigned long color, unsigned long columns, unsigned long rows) {
    unsigned long endRow = rows + row;
    unsigned long endColumn = columns + column;
    for (unsigned long y = row; y < endRow; y++) {
        for (unsigned long x = column; x < endColumn; x++) {
            long realColumn = GetRealColumn(x);
            FEListBoxCell *cell = &cells[GetRealRow(y) * numColumns + realColumn];
            cell->resourceIndex = color;
        }
    }
}
void FECodeListBox::SetCellScale(unsigned long column, unsigned long row, const FEPoint &scale, unsigned long columns, unsigned long rows) {
    unsigned long endRow = rows + row;
    unsigned long endColumn = columns + column;
    for (unsigned long y = row; y < endRow; y++) {
        for (unsigned long x = column; x < endColumn; x++) {
            long realColumn = GetRealColumn(x);
            FEListBoxCell *cell = &cells[GetRealRow(y) * numColumns + realColumn];
            cell->unknown_4.x = scale.x;
            cell->unknown_4.y = scale.y;
        }
    }
}
void FECodeListBox::SetCellJustification(unsigned long column, unsigned long row, unsigned long justify, unsigned long columns, unsigned long rows) {
    unsigned long endRow = rows + row;
    unsigned long endColumn = columns + column;
    for (unsigned long y = row; y < endRow; y++) {
        for (unsigned long x = column; x < endColumn; x++) {
            long realColumn = GetRealColumn(x);
            FEListBoxCell *cell = &cells[GetRealRow(y) * numColumns + realColumn];
            cell->unknown_1c = justify;
        }
    }
}
