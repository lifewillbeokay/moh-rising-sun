// Target-derived scoped views; see docs/research/broad-source-sweep.md.
#ifndef GAME_MEMORY_CARD_DEVICE_H
#define GAME_MEMORY_CARD_DEVICE_H
#include "MemoryCard.h"
namespace Realmc {
class Interface;
}
struct CMemoryCard::Device {
    static int ChangeOfState();
    static void Update(bool);
};
class CMCDevice {
  public:
    void *table;
    Realmc::Interface *interface;
    unsigned short slot, subslot;
    int status;
    char name[128];
    unsigned int freeBlocks, totalBlocks;
    mutable bool justInserted, justRemoved;
    const char *GetName() const;
    int GetIndex() const;
    void Invalidate(bool);
    void Cache(bool);
    int IsAvailable() const;
    int WasJustInserted() const;
    int WasJustRemoved() const;
    static int GetCount();
    static CMCDevice *GetDevice(unsigned int);
    const char *GetStatusText() const;
    int IsSelectable() const;
    void GetStats(unsigned int &, unsigned int &) const;
    int GetStatus() const;
};
extern CMCDevice gs_cards[2];
extern CMemoryCard *g_hMemoryCard;
#endif
