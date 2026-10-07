#include "PropertyData.h"
#include "loaders_internal.h"

struct _PropBSPNode;
// Descriptive header prefix: the root's offset is the word at +0x24.
struct PropertyBSPHeaderView {
    unsigned char unknown00[0x24];
    unsigned int rootOffset;
};
extern void *g_pPBSPHeader;
extern _PropBSPNode *g_pPBSPHead;
void PatchUpPropBSPTreeNode(char *, _PropBSPNode *);

void LoadPropBSPTree(char *filename, bool relocated) {
    PropertyBSPHeaderView *header = (PropertyBSPHeaderView *)TLT_LoadFileFromLevelBigFile(filename, 0);
    g_pPBSPHeader = header;
    g_pPBSPHead = (_PropBSPNode *)((char *)header + header->rootOffset);
    if (!relocated)
        PatchUpPropBSPTreeNode((char *)header, g_pPBSPHead);
}
