#ifndef GAME_MEMORYCARD_H
#define GAME_MEMORYCARD_H
// A scoped view of the memory card class. Class, method and global names come from the symbol
// table. The table's other slots hold pure virtual functions whose names are not recorded.
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
    virtual void Slot3() = 0;
    virtual void Slot4() = 0;
    virtual void Slot5() = 0;
    virtual void Slot6() = 0;
    virtual int Slot7Result() = 0;
    virtual void Slot8() = 0;
    virtual void Slot9() = 0;
    virtual void Slot10() = 0;
    virtual void *DoIssueTRCScan(ScanTask &, const Device *) = 0;
    virtual int DoCheckTRCScan() = 0;
    virtual void *IssueTRCScan(ScanTask &, const Device *);
    virtual int CheckTRCScan();
    virtual void Slot15() = 0;
    virtual void Slot16() = 0;
    virtual void Slot17() = 0;
    virtual int WasDataWritten() const;
    int IsBusy() const;
    void OnEnterInteractiveMode();
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
