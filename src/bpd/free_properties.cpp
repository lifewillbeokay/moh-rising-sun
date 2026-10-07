#include "PropertyData.h"
#include "AIFilterGlobal.h"
#include "StringTable.h"
#include "FlexPropDatabase.h"
extern BPDHeader *g_pBPDHeader;
extern void *g_pPBSPHeader;
void TLT_CloseFile(void *);
void FreePropertyMemory() {
    g_aigAIFilterGlobalObject.ShutdownSplinePathManager();
    delete g_pStringTable;
    g_pStringTable = 0;
    FlexPropDatabase::UnloadClasses();
    FlexPropDatabase::UnloadProperties();
    if (g_pPBSPHeader) {
        TLT_CloseFile(g_pPBSPHeader);
        g_pPBSPHeader = 0;
    }
    if (g_pBPDHeader) {
        TLT_CloseFile(g_pBPDHeader);
        g_pBPDHeader = 0;
    }
}
