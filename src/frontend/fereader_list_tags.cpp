// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
#include "FEngElements.h"
static inline float TagFloat(FETag *tag, int index) {
    union { unsigned long word; float value; } bits;
    bits.word = FETagLong(tag, index);
    return bits.value;
}
void FEPackageReader::ProcessListBoxTag(FETag *tag) {
    FEListBox *list = (FEListBox *)object;
    switch (FESwapShort(tag->id)) {
    case 0x644c:
        list->SetNumColumns(FETagLong(tag, 0));
        list->SetNumRows(FETagLong(tag, 1));
        columnIndex = cellIndex = rowIndex = -1;
        list->currentColumn = list->currentRow = 0;
        break;
    case 0x634c: {
        FEListEntryData &entry = list->columns[++columnIndex];
        entry.size = TagFloat(tag, 0); entry.unknown_8 = FETagLong(tag, 1); break;
    }
    case 0x724c: {
        FEListEntryData &entry = list->rows[++rowIndex];
        entry.size = TagFloat(tag, 0); entry.unknown_8 = FETagLong(tag, 1); break;
    }
    case 0x6343: {
        if (++cellIndex) list->IncrementCellByColumn();
        FEColor color(FETagLong(tag, 0));
        list->GetPCellData(list->currentColumn, list->currentRow)->resourceIndex = (unsigned long)color;
        break;
    }
    case 0x5443: list->SetCellType(FETagLong(tag, 0)); break;
    case 0x7443: list->SetCellString((const short *)(tag + 1)); break;
    case 0x774c: list->SetAutoWrap(FETagLong(tag, 0) != 0); break;
    case 0x734c: list->unknown_7c = TagFloat(tag, 0); list->unknown_80 = TagFloat(tag, 1); break;
    case 0x764c: list->unknown_64 = TagFloat(tag, 0); list->unknown_68 = TagFloat(tag, 1); break;
    case 0x7243: {
        unsigned long resource = FETagLong(tag, 0);
        FEListBoxCell *cell = list->GetPCellData(list->currentColumn, list->currentRow);
        cell->unknown_c = 0; cell->unknown_14 = resource; cell->unknown_10 = 0; break;
    }
    case 0x7343: {
        FEPoint scale(TagFloat(tag, 0), TagFloat(tag, 1));
        list->GetPCellData(list->currentColumn, list->currentRow)->unknown_4 = scale; break;
    }
    case 0x6943: {
        float values[4];
        for (int i = 0; i < 4; ++i) values[i] = TagFloat(tag, i);
        FEListBoxCell *cell = list->GetPCellData(list->currentColumn, list->currentRow);
        cell->unknown_20 = values[0]; cell->unknown_24 = values[1];
        cell->unknown_28 = values[2]; cell->unknown_2c = values[3]; break;
    }
    }
}
void FEPackageReader::ProcessCodeListBoxTag(FETag *tag) {
    FECodeListBox *list = (FECodeListBox *)object;
    switch (FESwapShort(tag->id)) {
    case 0x444c: list->Initialize(FETagLong(tag, 0), FETagLong(tag, 1)); break;
    case 0x4953: list->AllocateStrings(FETagLong(tag, 0), FETagLong(tag, 1)); break;
    case 0x6343: list->SetCellColor(0, 0, FETagLong(tag, 0), list->numColumns, list->numRows); break;
    case 0x6a4c: list->SetCellJustification(0, 0, FETagLong(tag, 0), list->numColumns, list->numRows); break;
    case 0x744c: list->listFlags = (list->listFlags & 1) | (FETagLong(tag, 0) & ~1); break;
    case 0x764c: list->unknown_80 = TagFloat(tag, 0); list->unknown_84 = TagFloat(tag, 1); break;
    case 0x7343: {
        FEPoint scale(TagFloat(tag, 0), TagFloat(tag, 1));
        list->SetCellScale(0, 0, scale, list->numColumns, list->numRows); break;
    }
    }
}
#endif
