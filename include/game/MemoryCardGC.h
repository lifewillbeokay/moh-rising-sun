// GR8E69 client-side card views; descriptive fields, not complete library classes.
// See docs/research/save-flow.md for offsets, virtual-slot evidence and unknowns.
#ifndef GAME_MEMORY_CARD_GC_H
#define GAME_MEMORY_CARD_GC_H
#pragma interface
#include "MemoryCardFlow.h"
#include "RealmcInterface.h"
#include "MohLoader.h"
namespace Realmc {
struct Message::DetailInfo {
    struct FileInfo {
        const char *name;
        unsigned char unknown04[8];
        char company[2];
        char game[4];
    };
};
extern const char *ROOT_DIRECTORY_NAME, *FILENAME_ALL_FILES;
}
struct CardFileInfoView {
    Realmc::FileInfo prefix; // +0..0x23; original client copies 48 bytes.
    const unsigned short *unknown24, *unknown28;
    int unknown2c;
};
struct CardMessageView {
    int id, result, trc;
    const unsigned short *text;
    int optionCount;
    unsigned char unknown14[16];
    const unsigned short *options[4]; // +0x24
    unsigned char unknown34[4];
    unsigned int blocks38, blocks3c;
    Realmc::Message::DetailInfo::FileInfo file; // +0x40
    void *descriptor; // +0x54
};
// Virtual-only view of the original _vt.Q26Realmc11GCInterface, including unused slots.
// Only called slots recover argument types; unused declarations reserve their table positions.
// No object of this type is constructed; it does not change the narrower library views.
class CardInterfaceView {
public:
    virtual int AddRef(); virtual int Release();
    virtual void TrcStartGame(const void *);
    virtual void TrcCardExists(const Realmc::CardID &);
    virtual void TrcGetCardInfo(const Realmc::CardID &);
    virtual void TrcLoadFile(const Realmc::CardID &, const CardFileInfoView &);
    virtual void TrcSaveFile(const Realmc::CardID &, const CardFileInfoView &);
    virtual void TrcDeleteFile(); virtual void TrcListFiles();
    virtual void CardExists(); virtual void GetCardInfo(const Realmc::CardID &);
    virtual void OpenFile(); virtual void CloseFile(); virtual void DeleteFile();
    virtual void Read(void *, void *, int); virtual void Write(void *, const void *, int);
    virtual void Seek(); virtual void Flush(); virtual void SetFileAttribute();
    virtual void FindFile(const Realmc::CardID &, const char *, const char *);
    virtual Realmc::Message *GetMessage(int);
    virtual void SendMessage(int, int); virtual void SetTrcMessageFile(); virtual int IsBusy();
    virtual const unsigned short *GetCardName(const Realmc::CardID &);
    virtual int GetBlockSize(const Realmc::CardID &); virtual void *GetBlockCalculator();
    virtual ~CardInterfaceView();
    virtual void Mount(const Realmc::CardID &); virtual void Unmount(const Realmc::CardID &);
    virtual void ClearTask();
};
class CMemoryCardGC : public CMemoryCard {
public:
    void Init();
    int CheckStart();
    void IssueSave(const char *, unsigned int); int CheckSave();
    void IssueLoad(const char *, unsigned int); int CheckLoad();
    void IssueScan(ScanTask &, const Device *); int CheckScan();
    void ProcessReply(unsigned int);
    void DisplayTRCMessage(const Realmc::Message *);
    bool IsOurs(const Realmc::Message::DetailInfo::FileInfo &) const;
    void TellTheGamecubeToShutUp(bool);
    int SaveSuccessful() const; int LoadSuccessful() const;
    int DefaultReply() const; int DeclinedToSave() const;
    CardInterfaceView *interface; // +8
    unsigned char startInfo[76]; // +0xc; not interpreted here.
    CardFileInfoView fileInfo; // +0x58
    int saveSuccess, loadSuccess, operationActive; // +0x88, 0x8c, 0x90
    ScanTask *scanTask; const Device *scanDevice; // +0x94, 0x98
    void *descriptor; int defaultReply; // +0x9c, 0xa0
};
inline const Realmc::CardID &CardIDOf(const CMemoryCard::Device *device) {
    return *(const Realmc::CardID *)&((const CMCDevice *)device)->slot;
}
extern int g_bVerbose;
extern CMemoryCardGC *g_pMCInstance;
extern const char *COMPANY_CODE, *GAME_CODE;
extern "C" { int TIMER_elapsedticks(); int TIMER_getfrequency(); void OSResetSystem(int, unsigned int, int); }
void MUSIC_Pause(bool); void SOUND_PauseAll(bool); void DebugMsg(const char *, ...);
#endif
