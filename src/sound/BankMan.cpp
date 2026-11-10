// A fragment of the original BankMan.cpp: the sound bank table. Names come from the symbol table. The
// table has 33 slots of bank handles, with -12 marking an empty slot; other member names are inferred.
#include <string.h>

extern "C" int SNDbankadd(int *bank, void *data);
extern "C" int SNDbankheadersize(int);
extern "C" int SNDbankheadercopy(void *buffer, int bank);
extern "C" int SNDbankremove(int);
extern "C" int SNDBANK_asyncdone();
extern "C" int SNDBANK_asyncload(char *, int, void *, int, void *(*)(int));
extern "C" void *DWI_alloc(const char *, int, int);

enum EUberBankTypes { UberBankUnknown };

class SoundEntryBank {
  public:
    unsigned char unknown00[0xac];
    unsigned int slotMask; // +0xac
    unsigned char unknownb0[0xdc - 0xb0];
    signed char field_dc; // +0xdc; counts the empty slots passed.
};

class BankMan {
  public:
    BankMan();
    int AddSoundBank(unsigned char *);
    void RemoveSoundBanks();
    void RemoveSoundBank(int);
    int GetBankHandle(SoundEntryBank &);
    int GetAsyncLoadStatus();
    int LoadAsyncSoundBank(int, char *, void *, int, void *(*)(int));
    int GetAsyncIsLoaded(EUberBankTypes);
    int LoadAsyncSoundBank(int, char *, void *(*)(int));
    int banks[33];
    void *headers[33];      // +0x84
    int count;              // +0x108
    int asyncLoading;       // +0x10c
    int asyncBank;          // +0x110
};

BankMan::BankMan() {
    count = 0;
    asyncBank = -1;
    asyncLoading = 0;
    memset(headers, 0, sizeof(headers));
    for (int i = 0; i < 33; i++) {
        banks[i] = -12;
        headers[i] = 0;
    }
}

int BankMan::AddSoundBank(unsigned char *data) {
    int bank;
    SNDbankadd(&bank, data);
    void *header = DWI_alloc("source/sound/BankMan.cpp:46", SNDbankheadersize(bank), 1024);
    if (!header)
        return -1;
    SNDbankheadercopy(header, bank);
    int slot = 31;
    banks[slot] = bank;
    headers[slot] = header;
    count++;
    return bank;
}

void BankMan::RemoveSoundBanks() {
    for (int i = 0; i < 33; i++) {
        if (banks[i] != -12)
            RemoveSoundBank(banks[i]);
    }
}

void BankMan::RemoveSoundBank(int bank) {
    for (int i = 0; i <= 32; i++) {
        if (banks[i] == bank) {
            SNDbankremove(banks[i]);
            banks[i] = -12;
            if (headers[i]) {
                delete[] (char *)headers[i];
                headers[i] = 0;
            }
            count--;
            return;
        }
    }
}

int BankMan::GetBankHandle(SoundEntryBank &entry) {
    unsigned int mask = entry.slotMask;
    for (int i = 1; i <= 32; i++) {
        if ((unsigned int)(i - 31) <= 1 || asyncLoading != 1) {
            if (mask & (1 << (i - 1))) {
                if (banks[i] != -12)
                    return banks[i];
                entry.field_dc++;
            }
        }
    }
    return -12;
}

int BankMan::GetAsyncLoadStatus() {
    if (!asyncLoading)
        return 0;
    int status = SNDBANK_asyncdone();
    if (status == 0)
        return 1;
    if (status > 0) {
        asyncLoading = 0;
        return 2;
    }
    asyncLoading = 0;
    return -1;
}

int BankMan::LoadAsyncSoundBank(int slot, char *name, void *buffer, int size, void *(*alloc)(int)) {
    if (asyncLoading == 1)
        return -1;
    if (slot > 30)
        return -1;
    int bank = SNDBANK_asyncload(name, 0, buffer, size, alloc);
    if (bank >= 0) {
        banks[slot] = bank;
        asyncBank = slot;
        asyncLoading = 1;
        count++;
    }
    return bank;
}

int BankMan::GetAsyncIsLoaded(EUberBankTypes type) {
    if ((unsigned int)type <= 32)
        return banks[type] != -12;
    return 0;
}

int BankMan::LoadAsyncSoundBank(int slot, char *data, void *(*alloc)(int)) {
    if (slot > 30)
        return -1;
    int bank;
    SNDbankadd(&bank, data);
    if (bank >= 0) {
        banks[slot] = bank;
        count++;
        void *header = alloc(SNDbankheadersize(bank));
        if (!header)
            return -1;
        SNDbankheadercopy(header, bank);
    }
    return bank;
}
