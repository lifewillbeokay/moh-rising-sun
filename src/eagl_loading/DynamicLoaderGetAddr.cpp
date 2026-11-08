// AI-assisted partial reconstruction from GR8E69; see docs/Loading.md and src/eagl_loading/NOTICE.
#if defined(DECOMP_COMPARE)
#include "EAGLDynamicLoader.h"
namespace EAGL {
bool DynamicLoader::GetAddr(const char *type, const char *name, void *&result) const {
    result = 0;
    HashPointer *h = (HashPointer *)handle;
    if (!h)
        return false;
    ELF32_Sym *s = h->symtab;
    unsigned long i = elfhash(name);
    i = h->hash[i];
    while (i != (unsigned long)-1) {
        if (i > h->symbols_num) {
            PrintMessage(
                0, "DynamicLoader::GetAddr() -- INTERNAL ERROR: Bad i value %d (chain size %d)!\n",
                i, h->symbols_num);
        } else {
            const char *symbolName = &h->strtab[s[i].st_name];
            const char *symbolType = symbolName + strlen(name) + 1;
            if (*symbolType == 127)
                symbolType++;
            else
                symbolType--;
            if (strcmp(name, symbolName) == 0 && strcmp(type, symbolType) == 0) {
                int section = s[i].st_shndx;
                if (s[i].st_other == 1) {
                    result = (void *)s[i].st_value;
                    return true;
                } else if (section > 0 && section < h->e->e_shnum) {
                    result = (void *)(h->sections[section].sh_offset + s[i].st_value);
                    return true;
                }
            }
        }
        i = h->chain[i];
    }
    if (mSearchCallback) {
        bool valid = false;
        result = mSearchCallback(name, valid);
        if (valid)
            return true;
    }
    return false;
}
} // namespace EAGL

#endif
