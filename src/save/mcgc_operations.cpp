// AI-assisted GR8E69 reconstruction; see docs/research/save-flow.md.
#include "MemoryCardGC.h"
#ifdef DECOMP_COMPARE
int CMemoryCardGC::CheckStart() {
    bool done = false, success = false, reset = false;
    const CardMessageView *m = (const CardMessageView *)interface->GetMessage(TIMER_elapsedticks() * 1000 / TIMER_getfrequency());
    char diagnostic[512];
    if (m) {
        switch (m->id) {
        case 23: interface->SendMessage(11, 0); break;
        case 1: done = true; if (m->result == 0) success = true; else if (m->result == 26) reset = true; break;
        case 24:
            DisplayTRCMessage((const Realmc::Message *)m);
            switch (m->trc) {
            case 11: case 12: case 14: case 15: case 16: case 17: defaultReply = 1; RequestReply(); break;
            case 13: defaultReply = 2; RequestReply(); break;
            case 26: case 35: RequestReply(); break;
            case 27: break;
            default: sprintf(diagnostic, "Unhandled TRC msg: %d\n", m->trc); break;
            }
            break;
        default: sprintf(diagnostic, "Unhandled msg: %d\n", m->id); break;
        }
    }
    if (reset) { OSResetSystem(1, 0, 1); return 2; }
    if (WaitingForReply()) { MohLoader::Instance()->resetEnabled = 1; return 1; }
    if (done) { operationActive = 0; MohLoader::Instance()->resetEnabled = 1; return success ? 2 : 3; }
    return 0;
}
extern CardFileInfoView saveFileInfo asm("fileInfo.3256");
extern int saveFileInfoGuard asm("_.tmp_1.3257");
void CMemoryCardGC::IssueSave(const char *name, unsigned int index) {
    MohLoader::Instance()->resetEnabled = 0;
    MUSIC_Pause(true); SOUND_PauseAll(true);
    operationActive = 1; descriptor = 0; saveSuccess = 0;
    if (!saveFileInfoGuard) { saveFileInfo = fileInfo; saveFileInfoGuard = 1; }
    saveFileInfo.prefix.name = name; saveFileInfo.prefix.comment = (char *)name;
    saveFileInfo.prefix.commentLength = strlen(name);
    CMCDevice *device = CMCDevice::GetDevice(index);
    interface->TrcSaveFile(*(const Realmc::CardID *)&device->slot, saveFileInfo);
    TIMER_elapsedticks();
}
int CMemoryCardGC::CheckSave() {
    bool done = false, retry = false;
    const CardMessageView *m = (const CardMessageView *)interface->GetMessage(TIMER_elapsedticks() * 1000 / TIMER_getfrequency());
    char diagnostic[512];
    if (m) {
        switch (m->id) {
        case 19: interface->SendMessage(1, 0); saveSuccess = 1; break;
        case 5: done = true; retry = m->result == 24; break;
        case 13: descriptor = m->descriptor; break;
        case 27: interface->Write(descriptor, GetScratchpad(), GetScratchpadSize()); break;
        case 24:
            DisplayTRCMessage((const Realmc::Message *)m);
            switch (m->trc) {
            case 11: case 12: case 13: case 14: case 15: case 16: case 18: case 19:
            case 21: case 26: case 32: case 35: RequestReply(); break;
            case 17: case 20: case 27: break;
            default: sprintf(diagnostic, "Unhandled TRC msg: %d\n", m->trc); break;
            }
            break;
        }
    }
    if (WaitingForReply()) { MohLoader::Instance()->resetEnabled = 1; return 1; }
    if (done) {
        if (retry) return 4;
        operationActive = 0; MohLoader::Instance()->resetEnabled = 1;
        MUSIC_Pause(false); SOUND_PauseAll(false); return 2;
    }
    return 0;
}
void CMemoryCardGC::IssueLoad(const char *name, unsigned int index) {
    MUSIC_Pause(true); SOUND_PauseAll(true);
    fileInfo.prefix.name = name; descriptor = 0; loadSuccess = 0; operationActive = 0;
    CMCDevice *device = CMCDevice::GetDevice(index);
    interface->TrcLoadFile(*(const Realmc::CardID *)&device->slot, fileInfo);
    TIMER_elapsedticks();
}
int CMemoryCardGC::CheckLoad() {
    bool done = false, retry = false;
    const CardMessageView *m = (const CardMessageView *)interface->GetMessage(TIMER_elapsedticks() * 1000 / TIMER_getfrequency());
    char diagnostic[512];
    if (m) {
        switch (m->id) {
        case 18: interface->SendMessage(2, 0); break;
        case 4: done = true; retry = m->result == 24; break;
        case 13: descriptor = m->descriptor; break;
        case 26: interface->Read(descriptor, GetScratchpad(), GetScratchpadSize()); break;
        case 28:
            if (ScratchpadDataOkay()) { interface->SendMessage(6, 0); loadSuccess = 1; }
            else interface->SendMessage(7, 0);
            break;
        case 24:
            if (m->trc != 23) DisplayTRCMessage((const Realmc::Message *)m);
            switch (m->trc) {
            case 11: case 12: case 13: case 14: case 22: case 25: case 26:
            case 28: case 33: case 34: case 35: RequestReply(); break;
            case 23: interface->SendMessage(10, 0); break;
            case 24: case 27: case 29: break;
            default: sprintf(diagnostic, "Unhandled TRC msg: %d\n", m->trc); break;
            }
            break;
        }
    }
    if (WaitingForReply()) return 1;
    if (done) {
        if (retry) return 4;
        MUSIC_Pause(false); SOUND_PauseAll(false); return 2;
    }
    return 0;
}
void CMemoryCardGC::IssueScan(ScanTask &task, const Device *device) {
    scanTask = &task; scanDevice = device;
    interface->Mount(CardIDOf(device)); TIMER_elapsedticks();
}
int CMemoryCardGC::CheckScan() {
    bool done = false;
    const CardMessageView *m = (const CardMessageView *)interface->GetMessage(TIMER_elapsedticks() * 1000 / TIMER_getfrequency());
    char diagnostic[512];
    if (m) {
        switch (m->id) {
        case 21:
            if (!m->result) interface->GetCardInfo(CardIDOf(scanDevice));
            else interface->Unmount(CardIDOf(scanDevice)); break;
        case 22: done = true; break;
        case 25:
            if (!m->result && IsOurs(m->file)) scanTask->Execute(m->file.name, scanDevice->GetIndex());
            break;
        case 9:
            if (!m->result) interface->FindFile(CardIDOf(scanDevice), Realmc::ROOT_DIRECTORY_NAME, Realmc::FILENAME_ALL_FILES);
            else interface->Unmount(CardIDOf(scanDevice)); break;
        case 12: interface->Unmount(CardIDOf(scanDevice)); break;
        default: sprintf(diagnostic, "Unhandled msg: %d\n", m->id); break;
        }
    }
    if (WaitingForReply()) return 1;
    if (done) { scanDevice = 0; scanTask = 0; return 2; }
    return 0;
}
void CMemoryCardGC::ProcessReply(unsigned int reply) {
    if (operationActive) MohLoader::Instance()->resetEnabled = 0;
    static const unsigned short blank[] = {32, 0};
    Display(blank, 0, 0, 0, 0);
    switch (reply) {
    case 1: interface->SendMessage(11, 0); break;
    case 0: interface->SendMessage(10, 0); break;
    case 2: interface->SendMessage(12, 0); break;
    case 3: interface->SendMessage(13, 0); break;
    default: DebugMsg("CMemoryCardGC::ProcessReply() received an unknown reply!\n"); break;
    }
}
void CMemoryCardGC::DisplayTRCMessage(const Realmc::Message *message) {
    const CardMessageView *m = (const CardMessageView *)message;
    const unsigned short *choices[4]; memset(choices, 0, sizeof(choices));
    if (m->optionCount > 0) choices[0] = m->options[0];
    if (m->optionCount > 1) choices[1] = m->options[1];
    if (m->optionCount > 2) choices[2] = m->options[2];
    if (m->optionCount > 3) choices[3] = m->options[3];
    Display(m->text, choices[0], choices[1], choices[2], choices[3]);
}
bool CMemoryCardGC::IsOurs(const Realmc::Message::DetailInfo::FileInfo &info) const {
    return info.company[0] == COMPANY_CODE[0] && info.company[1] == COMPANY_CODE[1]
        && info.game[0] == GAME_CODE[0] && info.game[1] == GAME_CODE[1]
        && info.game[2] == GAME_CODE[2] && info.game[3] == GAME_CODE[3] && strlen(info.name) < 9;
}

void CMemoryCardGC::TellTheGamecubeToShutUp(bool quiet) { g_bVerbose = !quiet; }
extern "C" unsigned short *wcsncpy(unsigned short *, const unsigned short *, unsigned int);
extern "C" unsigned int wcslen(const unsigned short *);
void CMCDevice::IssueCache() {
    CardInterfaceView *client = (CardInterfaceView *)interface;
    const unsigned short *label = client->GetCardName(*(const Realmc::CardID *)&slot);
    unsigned short *wideName = (unsigned short *)name;
    if (label) {
        wcsncpy(wideName, label, 63);
        for (unsigned int i = 0; i < wcslen(wideName); ++i)
            if (wideName[i] == 160) wideName[i] = 32;
    } else { wideName[0] = 32; wideName[1] = 0; }
    status = 0;
    ((CardInterfaceView *)interface)->TrcGetCardInfo(*(const Realmc::CardID *)&slot);
    TIMER_elapsedticks();
}
int CMCDevice::CheckCache() {
    bool done = false;
    const CardMessageView *m = (const CardMessageView *)((CardInterfaceView *)interface)->GetMessage(TIMER_elapsedticks() * 1000 / TIMER_getfrequency());
    char diagnostic[512];
    if (m) {
        switch (m->id) {
        case 24:
            if (g_bVerbose && m->trc != 11) g_pMCInstance->DisplayTRCMessage((const Realmc::Message *)m);
            switch (m->trc) {
            case 11:
                Invalidate(status != 1);
                ((CardInterfaceView *)interface)->SendMessage(11, 0); break;
            case 12: case 14:
                status = m->trc == 12 ? 4 : 2;
                if (g_bVerbose) { g_pMCInstance->defaultReply = 1; g_pMCInstance->RequestReply(); }
                else ((CardInterfaceView *)interface)->SendMessage(11, 0);
                break;
            case 13:
                status = 3;
                if (g_bVerbose) { g_pMCInstance->defaultReply = 2; g_pMCInstance->RequestReply(); }
                else ((CardInterfaceView *)interface)->SendMessage(12, 0);
                break;
            case 26: case 35: g_pMCInstance->defaultReply = 1; g_pMCInstance->RequestReply(); break;
            case 27: break;
            default: sprintf(diagnostic, "Unhandled TRC msg: %d\n", m->trc); break;
            }
            break;
        case 3:
            freeBlocks = m->blocks38; totalBlocks = m->blocks3c;
            if (totalBlocks > freeBlocks) totalBlocks = freeBlocks;
            done = true; break;
        default: sprintf(diagnostic, "Unhandled message %d!\n", m->id); break;
        }
    }
    return done ? 2 : 0;
}
#endif
