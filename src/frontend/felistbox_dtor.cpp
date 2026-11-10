// FEListBox: rows and columns of cells.
#include "FEListBox.h"
#include <string.h>

FEListBox::~FEListBox() { Terminate(); }
void FEListBox::Terminate() {
    if (((listFlags ^ 1) & 1) == 0) {
        listFlags &= ~1;
        CleanupColumns();
        CleanupRows();
        CleanupCells();
    }
}
