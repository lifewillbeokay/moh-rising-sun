// A fragment of the original drivecheck.cpp: polling the disc drive and showing its error messages.
// Names come from the symbol table; DiscDriveState's values are inferred from how they are used.
enum DiscDriveState { DiscDriveOk, DiscDriveBusy, DiscDriveNoDisc, DiscDriveCoverOpen, DiscDriveWrongDisc, DiscDriveFatal };
struct DiscErrorMessage {
    const char *lines[4];
};
extern DiscErrorMessage DiscErrorMsg_English[];
extern DiscErrorMessage DiscErrorMsg_French[];
extern DiscErrorMessage DiscErrorMsg_German[];
extern DiscErrorMessage DiscErrorMsg_Spanish[];
extern DiscErrorMessage DiscErrorMsg_Italian[];
extern DiscErrorMessage DiscErrorMsg_Dutch[];


extern "C" unsigned char OSGetLanguage();
extern "C" int DVDGetDriveStatus();
struct DVDDiskID {
    unsigned char unknown00[6];
    unsigned char diskNumber; // +0x06
};
extern "C" DVDDiskID *DVDGetCurrentDiskID();
extern "C" void VIWaitForRetrace();
void TEMP_DisplayMessage(const char *, const char *, const char *, const char *);
void DiscErrorCleared();
class MohLoaderGc {
  public:
    static MohLoaderGc *InstanceGc();
    int IsResetConsoleRequested();
    void ResetConsole();
};

// Each table has one four-line message per state.
#define SHOW(table) TEMP_DisplayMessage(table[state].lines[0], table[state].lines[1], table[state].lines[2], table[state].lines[3])

void DisplayMessage(DiscDriveState state) {
    switch ((signed char)OSGetLanguage()) {
    case 0:
        SHOW(DiscErrorMsg_English);
        break;
    case 3:
        SHOW(DiscErrorMsg_Spanish);
        break;
    case 4:
        SHOW(DiscErrorMsg_Italian);
        break;
    case 1:
        SHOW(DiscErrorMsg_German);
        break;
    case 2:
        SHOW(DiscErrorMsg_French);
        break;
    case 5:
        SHOW(DiscErrorMsg_Dutch);
        break;
    default:
        SHOW(DiscErrorMsg_English);
        break;
    }
}

DiscDriveState CheckDiscDriveStatus();

int CheckDiscDriveSyncTask(int, int) {
    bool previous = false;
    bool current;
    bool settled;
    do {
        DiscDriveState state = CheckDiscDriveStatus();
        MohLoaderGc *loader = MohLoaderGc::InstanceGc();
        if (loader->IsResetConsoleRequested())
            loader->ResetConsole();
        settled = (unsigned int)state <= DiscDriveBusy;
        current = previous | !settled;
        VIWaitForRetrace();
        previous = current;
    } while (!settled);
    if (current)
        DiscErrorCleared();
    return 0;
}

DiscDriveState CheckDiscDriveStatus() {
    static DiscDriveState laststate = DiscDriveOk;
    DiscDriveState state;
    switch ((unsigned int)DVDGetDriveStatus()) {
    case 1:
    case 2:
    case 7:
    case 8:
        state = DiscDriveBusy;
        break;
    case 5:
        state = DiscDriveNoDisc;
        break;
    case 4:
    case 6: {
        state = DiscDriveCoverOpen;
        if (DVDGetCurrentDiskID()->diskNumber == 1)
            state = DiscDriveWrongDisc;
        break;
    }
    case 11:
        state = DiscDriveFatal;
        break;
    case 0xffffffff:
        for (;;)
            ;
    case 0:
    case 9:
    case 10:
    default:
        state = DiscDriveOk;
        break;
    }
    if (state == DiscDriveBusy) {
        if ((unsigned int)(laststate - 3) <= 1)
            state = laststate;
    } else
        laststate = state;
    if ((unsigned int)state > 1)
        DisplayMessage(state);
    return state;
}

DiscDriveState CheckDiscChangeStatus() {
    DiscDriveState state;
    switch ((unsigned int)DVDGetDriveStatus()) {
    case 1:
    case 8:
        state = DiscDriveBusy;
        break;
    case 5:
        state = DiscDriveNoDisc;
        break;
    case 2:
    case 4:
    case 6:
    case 7:
        state = DiscDriveCoverOpen;
        if (DVDGetCurrentDiskID()->diskNumber == 0)
            state = DiscDriveWrongDisc;
        break;
    case 11:
        state = DiscDriveFatal;
        break;
    case 0xffffffff:
        for (;;)
            ;
    case 0:
    case 9:
    case 10:
    default:
        state = DiscDriveOk;
        break;
    }
    if ((unsigned int)state > 1)
        DisplayMessage(state);
    return state;
}
