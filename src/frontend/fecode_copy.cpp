// AI-assisted reconstruction from GR8E69; see docs/research/feng-animation-lists.md.
#ifdef DECOMP_COMPARE
#include "FEngElements.h"

FECodeListBox::FECodeListBox(const FECodeListBox &other, bool copyScripts)
    : FEObject(other, copyScripts), game(other.game), numColumns(0), numRows(0),
      listFlags(0), totalColumns(other.totalColumns), totalRows(other.totalRows),
      currentColumn(0), currentRow(0), targetColumn(0), targetRow(0),
      unknown_80(other.unknown_80), unknown_84(other.unknown_84), cells(0),
      unknown_8c(0), unknown_90(0), stringsInUse(0), strings(0), stringBlock(0),
      blink(1.0f), blinkRate(-0.00138888892f), selectColor(0xffffffff),
      selectCallback(other.selectCallback), unknown_bc(other.unknown_bc), unknown_c0(other.unknown_c0) {
    CopyProperties(other);
}

void FECodeListBox::CopyProperties(const FECodeListBox &other) {
    listFlags |= other.listFlags & 14;
    unknown_80 = other.unknown_80;
    unknown_84 = other.unknown_84;
    totalColumns = other.totalColumns;
    totalRows = other.totalRows;
    Initialize(other.numColumns, other.numRows);
    if (stringBlock) { delete[] stringBlock; stringBlock = 0; }
    if (strings) { delete[] strings; strings = 0; }
    unknown_90 = unknown_8c = stringsInUse = 0;
    AllocateStrings(other.unknown_8c, other.unknown_90);
    unsigned long count = numColumns * numRows;
    for (unsigned long i = 0; i < count; ++i) {
        cells[i].resourceIndex = other.cells[i].resourceIndex;
        cells[i].type = other.cells[i].type;
        cells[i].unknown_4 = other.cells[i].unknown_4;
        cells[i].unknown_c = other.cells[i].unknown_c;
        cells[i].unknown_14 = other.cells[i].unknown_14;
        cells[i].unknown_10 = other.cells[i].unknown_10;
        cells[i].unknown_1c = other.cells[i].unknown_1c;
        if (cells[i].type == 2) {
            const short *source = other.cells[i].text;
            if (!source) return;
            cells[i].text = AllocateString();
            CopyString<short>(cells[i].text, source);
        }
        if (cells[i].type == 1) {
            cells[i].unknown_20 = other.cells[i].unknown_20;
            cells[i].unknown_24 = other.cells[i].unknown_24;
            cells[i].unknown_28 = other.cells[i].unknown_28;
            cells[i].unknown_2c = other.cells[i].unknown_2c;
        }
    }
}

#endif
