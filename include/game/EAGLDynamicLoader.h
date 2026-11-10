// Scoped loader interface and ELF32 storage; see docs/Loading.md and src/eagl_loading/NOTICE.
#ifndef GAME_EAGL_DYNAMIC_LOADER_H
#define GAME_EAGL_DYNAMIC_LOADER_H
#include "EAGLLoading.h"
namespace EAGL {
struct DestructorEntry {
    Destructor d;
    void *data;
};
struct RuntimeAllocDestructorEntry {
    RuntimeAllocDestructor d;
    void *data;
    int auxData;
    RuntimeAllocDestructorEntry *next;
    static void operator delete(void *p, unsigned n) { EAGLInternal::EAGLFree(p, n); }
};
class DynamicLoader {
  public:
    static const char AnimBankType[]; // A symbol type name.
    struct Symbol {
        Symbol() : name(0), data(0) {}
        const char *name, *type;
        void *data;
        bool isInternalRef;
    };
    void *handle;
    int nDestructors;
    DestructorEntry *destructors;
    RuntimeAllocDestructorEntry *RuntimeAllocDestructors;
    char *mpData;
    unsigned mDataLen;
    char *mpReloc;
    DynamicUserCallback mSearchCallback;
    int mNumPatchAddresses, mMaxPatchAddresses;
    unsigned **mpPatchAddresses32;
    bool mIsResolved;
    static void operator delete(void *p, unsigned n) { EAGLInternal::EAGLFree(p, n); }
    static const char ShapeType[6];
    static SymbolPool gSymbolPool;
    static ConstructorPool gConsPool;
    static RuntimeAllocConstructorPool gRuntimeAllocConsPool;
    DynamicLoader(void *, unsigned, void *, bool, DynamicUserCallback);
    ~DynamicLoader();
    bool DoVersionCheck();
    void Initialize(DynamicUserCallback);
    void Resolve();
    void *ELFAddr(unsigned int offset) {
        if (offset < mDataLen)
            return mpData + offset;
        if (mpReloc)
            return mpReloc + offset - mDataLen;
        if (offset > mDataLen)
            return 0;
        return mpData + offset;
    }
    void Release();
    void RunConstructors();
    void RunDestructors();
    int GetCount() const;
    void *GetElfData() const;
    Symbol GetSymbol(int) const;
    bool GetAddr(const char *, const char *, void *&) const;
    bool GetNextSymbol(const char *, int &, Symbol &) const;
    bool GetNextAddr(const char *, int &, void *&) const;
    static const char *RegisterVar(const char *, void *);
    static void UnRegisterVar(const char *);
    static void *GetRegisteredVar(const char *, bool &);
};
typedef unsigned int Elf32_Addr;
typedef short unsigned int Elf32_Half;
typedef unsigned int Elf32_Off;
typedef int Elf32_Sword;
typedef unsigned int Elf32_Word;

struct ELFHeader { // 0x34
    /* 0x00 */ unsigned char e_ident[16];
    /* 0x10 */ Elf32_Half e_type;
    /* 0x12 */ Elf32_Half e_machine;
    /* 0x14 */ Elf32_Word e_version;
    /* 0x18 */ Elf32_Addr e_entry;
    /* 0x1c */ Elf32_Off e_phoff;
    /* 0x20 */ Elf32_Off e_shoff;
    /* 0x24 */ Elf32_Word e_flags;
    /* 0x28 */ Elf32_Half e_ehsize;
    /* 0x2a */ Elf32_Half e_phentsize;
    /* 0x2c */ Elf32_Half e_phnum;
    /* 0x2e */ Elf32_Half e_shentsize;
    /* 0x30 */ Elf32_Half e_shnum;
    /* 0x32 */ Elf32_Half e_shstrndx;
};

struct ELFSectionHeader { // 0x28
    /* 0x00 */ Elf32_Word sh_name;
    /* 0x04 */ Elf32_Word sh_type;
    /* 0x08 */ Elf32_Word sh_flags;
    /* 0x0c */ Elf32_Addr sh_addr;
    /* 0x10 */ union { // 0x4
        /* 0x10 */ Elf32_Off sh_offset;
        /* 0x10 */ void *sh_voffset;
    };
    /* 0x14 */ Elf32_Word sh_size;
    /* 0x18 */ union { // 0x4
        /* 0x18 */ Elf32_Word sh_link;
        /* 0x18 */ void *sh_vlink;
    };
    /* 0x1c */ union { // 0x4
        /* 0x1c */ Elf32_Word sh_info;
        /* 0x1c */ void *sh_vinfo;
    };
    /* 0x20 */ Elf32_Word sh_addralign;
    /* 0x24 */ Elf32_Word sh_entsize;
};

struct ELF32_Sym { // 0x10
    /* 0x0 */ Elf32_Word st_name;
    /* 0x4 */ Elf32_Addr st_value;
    /* 0x8 */ Elf32_Word st_size;
    /* 0xc */ unsigned char st_info;
    /* 0xd */ unsigned char st_other;
    /* 0xe */ Elf32_Half st_shndx;
};

struct ELF32_Rel { // 0x8
    /* 0x0 */ Elf32_Addr r_offset;
    /* 0x4 */ Elf32_Word r_info;
};

struct ELF32_Rela { // 0xc
    /* 0x0 */ Elf32_Addr r_offset;
    /* 0x4 */ Elf32_Word r_info;
    /* 0x8 */ Elf32_Sword r_addend;
};

struct HashPointer {

    void operator delete(void *ptr, size_t size) { EAGLInternal::EAGLFree(ptr, size); }

    bool IsResolved() { return resolved; }

    DynamicLoader &GetDynamicLoader() { return *mpDynamicLoader; }

    HashPointer(DynamicLoader *pDL) { mpDynamicLoader = pDL; }

    ~HashPointer() {}

    HashPointer *next;
    HashPointer *prev;
    char *strtab;
    bool resolved;
    int symbols_num;
    ELF32_Sym *symtab;
    ELFSectionHeader *sections;
    ELFHeader *e;
    long unsigned int hash[256];
    long unsigned int *chain;
    bool *isOriginal;
    DynamicLoader *mpDynamicLoader;
    DynamicUserCallback pSearchFunction;
};
extern HashPointer *hashhead;
int PrintMessage(int, const char *, ...);
static inline unsigned long elfhash(const char *name) {
    unsigned long h = 0, g;
    while (*name) {
        h = h * 16 + *name;
        name++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h & 255;
}
} // namespace EAGL
#endif
