// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "GameSettings.h"
#ifdef DECOMP_COMPARE
void CGameSettings::SetUpButtonConfig(int setting, short *buttons) {
    switch (setting) {
    case 1:
        buttons[13] = 0x200;
        buttons[0] = 0x12; buttons[1] = 0x10; buttons[2] = 0x11; buttons[3] = 0x15;
        buttons[4] = 0x800; buttons[5] = 4; buttons[6] = 6; buttons[7] = 7;
        buttons[9] = 0x400; buttons[10] = 1; buttons[11] = 2; buttons[12] = -1; buttons[8] = 2;
        break;
    case 2: case 3:
        buttons[13] = 0x10;
        buttons[0] = 0x12; buttons[2] = 0xf; buttons[3] = 0x11; buttons[4] = 0x800;
        buttons[5] = 0x400; buttons[6] = 0; buttons[9] = 0x100; buttons[10] = 0x200;
        buttons[11] = 1; buttons[12] = -1; buttons[1] = 0x10; buttons[7] = 1; buttons[8] = 1;
        break;
    default:
        buttons[13] = 0x10;
        buttons[0] = 0x12; buttons[2] = 0xf; buttons[3] = 0x11; buttons[4] = 0x800;
        buttons[5] = 0x200; buttons[6] = 0; buttons[9] = 0x400; buttons[10] = 1;
        buttons[11] = 0x100; buttons[12] = -1; buttons[1] = 0x10; buttons[7] = 1; buttons[8] = 0x100;
        break;
    }
}
#endif
