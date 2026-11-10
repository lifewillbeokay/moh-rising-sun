// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEListBox.h"
#include <string.h>

void FEListBox::SetNumColumns(unsigned long count) {
    if (!count) { CleanupColumns(); CleanupCells(); return; }
    FEListEntryData *entries = new ("source/FEngine/FEListBox.cpp:263") FEListEntryData[count];
    unsigned long keep = 0;
    if (numColumns) {
        keep = numColumns < count ? numColumns : count;
        memcpy(entries, columns, keep * sizeof(FEListEntryData));
        if (columns) delete[] columns;
    }
    InitializeListEntry(entries + keep, count - keep);
    FEListBoxCell *replacement = 0;
    unsigned long total = count * numRows;
    if (total) {
        replacement = new ("source/FEngine/FEListBox.cpp:287") FEListBoxCell[total];
        if (cells) {
            for (unsigned long row = 0; row < numRows; ++row) {
                // This is a byte count in the original, not keep * sizeof(cell).
                memcpy(replacement + row * count, cells + row * numColumns, keep);
                InitializeCell(replacement + row * count + keep, count - keep);
            }
            if (cells) delete[] cells;
        } else InitializeCell(replacement, total);
    }
    cells = replacement;
    numColumns = count;
    columns = entries;
}

void FEListBox::SetNumRows(unsigned long count) {
    if (!count) { CleanupRows(); CleanupCells(); return; }
    FEListEntryData *entries = new ("source/FEngine/FEListBox.cpp:339") FEListEntryData[count];
    unsigned long keep = 0;
    if (numRows) {
        keep = numRows < count ? numRows : count;
        memcpy(entries, rows, keep * sizeof(FEListEntryData));
        if (rows) delete[] rows;
    }
    InitializeListEntry(entries + keep, count - keep);
    FEListBoxCell *replacement = 0;
    unsigned long total = numColumns * count;
    if (total) {
        replacement = new ("source/FEngine/FEListBox.cpp:363") FEListBoxCell[total];
        if (cells) {
            // Preserve the original byte-count behavior, including partial cells.
            memcpy(replacement, cells, keep * numColumns);
            InitializeCell(replacement + keep * numColumns, (count - keep) * numColumns);
            if (cells) delete[] cells;
        } else InitializeCell(replacement, total);
    }
    cells = replacement;
    numRows = count;
    rows = entries;
}

#endif
