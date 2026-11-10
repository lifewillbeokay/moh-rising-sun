// AI-assisted reconstruction of the sound manager's bank table load; see docs/Sound.md.
#include "SoundManager.h"

int cSoundMan::LoadLevelUbr(UberFileHeader *file) {
    int offset = file->columnCount;
    columnCount = offset;
    offset *= sizeof(ColumnMap);
    offset += sizeof(UberFileHeader);
    columns = (ColumnMap *)(file + 1);
    rowCount = file->rowCount;
    rowSize = file->rowSize;
    rows = (unsigned char *)file + offset;
    slotMap = (short *)(rows + rowSize * rowCount);
    if (file->hasBankTable) {
        int skip = file->slotCount * sizeof(short);
        SoundBankTable *table = (SoundBankTable *)((unsigned char *)slotMap + skip);
        bankTable = *table;
        hasBankTable = 1;
    } else {
        hasBankTable = 0;
    }
    CreateRowCRCMap();
    return 1;
}
