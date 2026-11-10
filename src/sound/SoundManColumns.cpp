// AI-assisted reconstruction of the sound manager's alias column lookups; see docs/Sound.md.
#include "SoundManager.h"

void cSoundMan::CreateRowCRCMap() {
    for (int i = 0; i < rowCount; i++) {
        aliases[i].row = rows + i * rowSize;
        aliases[i].crc = GetUbrCRC(aliases[i].row, SoundUberTask_Sound);
    }
}

void cSoundMan::CreateBusRowCRCMap() {
    for (int i = 0; i < bussRowCount; i++) {
        bussAliases[i].row = bussRows + i * bussRowSize;
        bussAliases[i].crc = GetBussCRC(bussAliases[i].row, SoundUberTask_Buss);
    }
}

unsigned int cSoundMan::GetUbrCRC(unsigned char *row, ESoundUberTask task) {
    int i;
    for (i = 0; i < columnCount; i++) {
        if (task == SoundUberTask_Sound && columns[i].type == 6)
            break;
    }
    unsigned int crc = 0;
    if (task == SoundUberTask_Sound)
        crc = *(unsigned int *)(row + columns[i].offset);
    return crc;
}

unsigned int cSoundMan::GetBussCRC(unsigned char *row, ESoundUberTask task) {
    int i;
    for (i = 0; i < bussColumnCount; i++) {
        if (task == SoundUberTask_Buss && bussColumns[i].type == 6)
            break;
    }
    unsigned int crc = 0;
    if (task == SoundUberTask_Buss)
        crc = *(unsigned int *)(row + bussColumns[i].offset);
    return crc;
}
