#ifndef GAME_MEMORYCARD_H
#define GAME_MEMORYCARD_H
// A scoped view of the memory card class. Class, method and global names come from the symbol
// table. Pure-virtual slot names follow the corresponding CMemoryCardGC overrides.
// DoIssueTRCScan and DoCheckTRCScan retain the older descriptive names for slots 11/12.
namespace EALA {
class Resource {
public:
    Resource() : unknown00(0) {}
    virtual ~Resource();
    int unknown00;
};
}

class CMemoryCard : public EALA::Resource {
public:
    struct ScanTask;
    struct Device;
    CMemoryCard();
    virtual ~CMemoryCard();
    virtual void TellTheGamecubeToShutUp(bool);
    virtual void IssueStart() = 0;
    virtual int CheckStart() = 0;
    virtual void IssueSave(const char *, unsigned int) = 0;
    virtual int CheckSave() = 0;
    virtual int SaveSuccessful() const = 0;
    virtual void IssueLoad(const char *, unsigned int) = 0;
    virtual int CheckLoad() = 0;
    virtual int LoadSuccessful() const = 0;
    virtual void *DoIssueTRCScan(ScanTask &, const Device *) = 0;
    virtual int DoCheckTRCScan() = 0;
    virtual void *IssueTRCScan(ScanTask &, const Device *);
    virtual int CheckTRCScan();
    virtual void ProcessReply(unsigned int) = 0;
    virtual int DefaultReply() const = 0;
    virtual int DeclinedToSave() const = 0;
    virtual int WasDataWritten() const;
    int IsBusy() const;
    void OnEnterInteractiveMode();
    int OnStartup();
    int Save(struct SPlayerProfile &);
    void Wait();
    bool ScratchpadDataOkay() const;
    void RequestReply();
    // PCUw records a wide-character ABI whose original source spelling is unknown.
    void Display(const unsigned short *, const unsigned short *, const unsigned short *, const unsigned short *, const unsigned short *) asm("Display__11CMemoryCardPCUwN41");
    int ScanVisibly(ScanTask &, int);
    int Load(const char *, unsigned int, struct SPlayerProfile &);
    void CancelReplyRequest();
    int WaitingForReply() const;
    void *GetTextDatabase() const;
    void *GetMiscTextDatabase() const;
    void *GetScratchpad() const;
    unsigned int GetScratchpadSize() const;
    static void Free(void *);
    void Cache(Device *);
    int GetDeviceCount() const;
    const Device *GetDevice(unsigned int) const;
};
#endif
