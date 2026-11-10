// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#include "FEListBox.h"

FEListBox::FEListBox() : listFlags(4), numColumns(0), numRows(0), unknown_64(0.0f),
    unknown_68(0.0f), scrollX(0.0f), scrollY(0.0f), columns(0), rows(0),
    unknown_7c(1.0f), unknown_80(1.0f), currentColumn(0), currentRow(0), cells(0),
    targetX(0.0f), targetY(0.0f), unknown_98(0.0f), unknown_9c(0.0f),
    blink(1.0f), blinkRate(-0.00138888892f) {
    type = 4;
}
