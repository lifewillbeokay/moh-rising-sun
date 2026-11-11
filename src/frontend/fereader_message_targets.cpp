// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEPackageReader.h"
#include "FEngStrings.h"
int FEPackageReader::ReadMessageTargetListChunk() {
    FEChunk *chunk = FindChild(root, 0x67726154);
    if (chunk) {
        char *end = (char *)(chunk + 1) + FESwap(chunk->size);
        unsigned long index = 0;
        for (FETag *tag = (FETag *)(chunk + 1); (char *)tag < end;
             tag = (FETag *)((char *)(tag + 1) + FESwapShort(tag->size))) {
            switch (FESwapShort(tag->id)) {
            case 0x6354:
                package->messageTargetCount = FETagLong(tag, 0);
                package->messageTargets = new ("source/FEngine/FEPackageReader.cpp:1225") FEMsgTargetList[package->messageTargetCount];
                break;
            case 0x744d: {
                FEMsgTargetList &list = package->messageTargets[index++];
                list.message = FETagLong(tag, 0);
                unsigned long count = (FESwapShort(tag->size) >> 2) - 1;
                list.Allocate(count);
                for (unsigned long i = 0; i < count; ++i)
                    list.AppendTarget(package->FindObjectByGUID(FETagLong(tag, i + 1)));
                break;
            }
            }
        }
    }
    return 1;
}
#endif
