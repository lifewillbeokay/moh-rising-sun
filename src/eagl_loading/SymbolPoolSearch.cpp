// AI-assisted partial reconstruction from GR8E69; see docs/Loading.md and src/eagl_loading/NOTICE.
#if defined(DECOMP_COMPARE)
#include "EAGLLoading.h"
namespace EAGL {
void *SymbolPool::Search(const char *Name, bool &Valid) {
    if (!mpSymbolTable) {
        Valid = false;
        return 0;
    }
    unsigned index = HashFunction(Name);
    int count = 0;
    EAGLInternal::SymbolEntry *found, *pSymbol;
    while (!(pSymbol = mpSymbolTable[index]) || strcmp(Name, (char *)(pSymbol + 1)) != 0 ||
           !(found = mpSymbolTable[index])) {
        if (count > mTableLength)
            goto end;
        index++;
        if (index >= mTableLength)
            index = 0;
        count++;
    }
    Valid = true;
    return found->Address;
end:
    Valid = false;
    return 0;
}
} // namespace EAGL

#endif
