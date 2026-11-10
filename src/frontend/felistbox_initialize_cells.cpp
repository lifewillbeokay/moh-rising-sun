// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEListBox.h"

void FEListBox::InitializeCell(FEListBoxCell *cells, unsigned long count) {
    for (unsigned long i = 0; i < count; ++i) {
        cells[i].resourceIndex = -1;
        cells[i].unknown_4 = FEPoint(1.0f, 1.0f);
        cells[i].unknown_c = 0;
        cells[i].unknown_10 = 0;
        cells[i].unknown_14 = 0;
        cells[i].type = 0;
        cells[i].unknown_20 = 0.0f;
        cells[i].unknown_24 = 0.0f;
        cells[i].unknown_28 = 1.0f;
        cells[i].unknown_2c = 1.0f;
        cells[i].unknown_1c = 0;
    }
}

#endif
