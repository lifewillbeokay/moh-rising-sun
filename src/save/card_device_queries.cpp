// AI-assisted reconstruction from GR8E69; names beyond symbols are descriptive.
#include "MemoryCardDevice.h"
#include "Objectives.h"
#include <stdio.h>
int CMCDevice::IsAvailable() const { return 1; }
int CMCDevice::WasJustInserted() const {
    int value = justInserted;
    justInserted = 0;
    return value;
}
int CMCDevice::WasJustRemoved() const {
    int value = justRemoved;
    justRemoved = 0;
    return value;
}
int CMCDevice::GetCount() { return 2; }
CMCDevice *CMCDevice::GetDevice(unsigned int index) { return gs_cards + index; }
const char *CMCDevice::GetStatusText() const {
    static char buffer[128];
    sprintf(buffer, " ");
    int message = 0;
    switch (status) {
    case 1:
        message = 1146;
        break;
    case 0:
        message = 1138;
        break;
    case 5:
        message = 1140;
        break;
    case 4:
        message = 1143;
        break;
    case 2:
        message = 1144;
        break;
    case 3:
        message = 1145;
        break;
    }
    if (message) {
        const char *format = g_pPopUpMessageHandler->tables[0].GetString(message);
        if (format)
            sprintf(buffer, format, totalBlocks, freeBlocks);
    }
    return buffer;
}
int CMCDevice::IsSelectable() const { return 1; }
