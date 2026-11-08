// AI-assisted partial reconstruction from GR8E69; see docs/Loading.md and src/eagl_loading/NOTICE.
#if defined(DECOMP_COMPARE)
#include "EAGLDynamicLoader.h"
namespace EAGL {
static inline unsigned short Swap16(unsigned short v) { return (v << 8) | (v >> 8); }
static inline unsigned Swap32(unsigned v) {
    return (unsigned)Swap16(v & 65535) << 16 | Swap16(v >> 16);
}
static inline void NoSectionWork() {}
void DynamicLoader::Initialize(DynamicUserCallback search) {
    HashPointer *h = (HashPointer *)EAGLInternal::EAGLMalloc(sizeof(HashPointer), 0);
    h->pSearchFunction = 0;
    h->e = 0;
    h->mpDynamicLoader = this;
    h->next = 0;
    h->strtab = 0;
    h->resolved = false;
    h->symbols_num = 0;
    h->symtab = 0;
    h->sections = 0;
    h->chain = 0;
    h->isOriginal = 0;
    ELFHeader *e = (ELFHeader *)mpData;
    h->e = e;
    e->e_type = Swap16(e->e_type);
    e->e_machine = Swap16(e->e_machine);
    e->e_version = Swap32(e->e_version);
    e->e_entry = Swap32(e->e_entry);
    e->e_phoff = Swap32(e->e_phoff);
    e->e_shoff = Swap32(e->e_shoff);
    e->e_flags = Swap32(e->e_flags);
    e->e_ehsize = Swap16(e->e_ehsize);
    e->e_phentsize = Swap16(e->e_phentsize);
    e->e_phnum = Swap16(e->e_phnum);
    e->e_shentsize = Swap16(e->e_shentsize);
    e->e_shnum = Swap16(e->e_shnum);
    e->e_shstrndx = Swap16(e->e_shstrndx);
    ELFSectionHeader *sections = (ELFSectionHeader *)ELFAddr(e->e_shoff);
    h->symtab = 0;
    h->sections = sections;
    int i;
    for (i = 0; i < e->e_shnum; i++) {
        ELFSectionHeader *s = &sections[i];
        s->sh_name = Swap32(s->sh_name);
        s->sh_type = Swap32(s->sh_type);
        s->sh_flags = Swap32(s->sh_flags);
        s->sh_addr = Swap32(s->sh_addr);
        s->sh_offset = Swap32(s->sh_offset);
        s->sh_size = Swap32(s->sh_size);
        s->sh_link = Swap32(s->sh_link);
        s->sh_info = Swap32(s->sh_info);
        s->sh_addralign = Swap32(s->sh_addralign);
        s->sh_entsize = Swap32(s->sh_entsize);
        s->sh_voffset = ELFAddr(s->sh_offset);
    }
    char *names = (char *)sections[e->e_shstrndx].sh_voffset;
    for (i = 0; i < e->e_shnum; i++) {
        ELFSectionHeader *s = &sections[i];
        switch (s->sh_type) {
        case 0:
        case 1:
        case 0x70000005:
        case 0x70000006:
        case 0x7ffff420:
        case 0x7ffff421:
            NoSectionWork();
            break;
        case 2:
            s->sh_vlink = sections[s->sh_link].sh_voffset;
            if (strcmp(".symtab", names + s->sh_name) == 0) {
                h->symtab = (ELF32_Sym *)s->sh_voffset;
                h->symbols_num = (int)s->sh_size / (int)sizeof(ELF32_Sym);
            }
            break;
        case 3:
            if (strcmp(".strtab", names + s->sh_name) == 0) {
                h->strtab = (char *)s->sh_voffset;
                char *p = h->strtab;
                int remaining = s->sh_size;
                const char *delimiter = ":::";
                while (remaining > 0) {
                    int len = strlen(p);
                    if (p[0] == '_' && p[1] == '_') {
                        char *separator = strstr(p + 2, delimiter);
                        if (separator) {
                            char type[128];
                            *separator = 0;
                            strcpy(type, p + 2);
                            char *name = separator + strlen(delimiter);
                            int n = strlen(name) + 1;
                            memmove(p, name, n);
                            p[n] = 127;
                            strcpy(p + n + 1, type);
                        }
                    }
                    p += len + 1;
                    remaining -= len + 1;
                }
            }
            break;
        case 4:
            if (s->sh_size)
                PrintMessage(0, "dlopen: non empty SHT_RELA section seen. Not supported.\n");
            break;
        case 5:
        case 6:
            PrintMessage(0, "dlopen: SHT_HASH section seen. Not supported.\n");
            break;
        case 9:
            s->sh_vinfo = sections[s->sh_info].sh_voffset;
            s->sh_vlink = sections[s->sh_link].sh_voffset;
            break;
        case 8:
            if (s->sh_size)
                PrintMessage(0, "dlopen: SHT_NOBITS section with data seen. Not supported.\n");
            break;
        case 10:
            PrintMessage(0, "dlopen: SHT_SHLIB section seen. Not supported.\n");
            break;
        case 11:
            PrintMessage(0, "dlopen: SHT_DYNSYM section seen. Not supported.\n");
            break;
        default:
            PrintMessage(0, "Unknown section type %d (section %d). Aborting dynamic loading.\n",
                         s->sh_type, i);
            break;
        }
    }
    if (!h->symtab)
        PrintMessage(0, "dlopen: Failed to find a symbol table. Aborting\n");
    h->pSearchFunction = search;
    for (i = 255; i >= 0; i--)
        h->hash[i] = -1;
    h->chain = (unsigned long *)EAGLInternal::EAGLMalloc(h->symbols_num * sizeof(unsigned long),
                                                         "EAGL::dynamic symbols");
    h->isOriginal =
        (bool *)EAGLInternal::EAGLMalloc(h->symbols_num * sizeof(bool), "EAGL::dynamic symbols 2");
    for (i = 0; i < h->symbols_num; i++) {
        ELF32_Sym *s = h->symtab;
        s[i].st_name = Swap32(s[i].st_name);
        s[i].st_value = Swap32(s[i].st_value);
        s[i].st_size = Swap32(s[i].st_size);
        s[i].st_shndx = Swap16(s[i].st_shndx);
        unsigned long hash = elfhash(h->strtab + s[i].st_name);
        h->chain[i] = h->hash[hash];
        h->hash[hash] = i;
        unsigned index = s[i].st_shndx;
        if (index > 0 && index < e->e_shnum)
            h->isOriginal[i] = true;
        else
            h->isOriginal[i] = false;
    }
    h->next = hashhead;
    if (hashhead)
        hashhead->prev = h;
    h->prev = 0;
    hashhead = h;
    handle = h;
}
} // namespace EAGL

#endif
