// FEPackageReader.cpp: the chunk readers.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEListBox.h"
#include "FEngElements.h"
#include "FEScript.h"
#include "ShellManager.h"
#include <string.h>
inline void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }
void FEKeyInterp(FEScript *, unsigned char, long, FEObject *);

int FEPackageReader::ReadLibraryRefsChunk() {
    FEChunk *chunk = FindChild(root, 0x5262694c);
    if (!chunk) return package->referencedPackages.count == 0;
    unsigned long size = FESwap(chunk->size);
    unsigned long count = size / 12;
    if (size != count * 12) return 0;
    FEFileLibraryRef *in = (FEFileLibraryRef *)(chunk + 1);
    package->SetNumLibraryRefs(count);
    FELibraryReference *out = package->libraryRefs;
    for (unsigned long i = 0; i < count; i++) {
        out->id = FESwap(in->id);
        out->unknown_4 = FESwap(in->unknown_4);
        out->unknown_8 = FESwap(in->unknown_8);
        in++;
        out++;
    }
    return 1;
}
int FEPackageReader::ReadResourceChunk() {
    FEChunk *chunk = FindChild(root, 0xcc736552);
    if (!chunk) return 0;
    FEChunk *names = chunk + 1;
    if (FESwap(names->id) != 0x6d4e7352) return 0;
    FEChunk *requests = (FEChunk *)((char *)names + FESwap(names->size) + 8);
    if (FESwap(requests->id) != 0x71527352) return 0;
    unsigned long *in = (unsigned long *)(requests + 1);
    unsigned long count = FESwap(*in++);
    package->resourceCount = count;
    if (count) {
        package->resourceRequests = new ("source/FEngine/FEPackageReader.cpp:323") FEResourceRequest[count];
        package->resourceNames = new ("source/FEngine/FEPackageReader.cpp:324") char[FESwap(names->size)];
        void *nameData = names + 1;
        for (unsigned long i = 0; i < count; i++) {
            package->resourceRequests[i].id = FESwap(in[0]);
            package->resourceRequests[i].name = (char *)FESwap(in[1]);
            package->resourceRequests[i].type = FESwap(in[2]);
            package->resourceRequests[i].unknown_c = FESwap(in[3]);
            package->resourceRequests[i].handle = (void *)FESwap(in[4]);
            package->resourceRequests[i].data = (void *)FESwap(in[5]);
            in += 6;
        }
        void *nameCopy = package->resourceNames;
        memcpy(nameCopy, nameData, FESwap(names->size));
        for (unsigned long j = 0; j < count; j++) {
            package->resourceRequests[j].name += (unsigned long)package->resourceNames;
        }
    }
    return 1;
}
int FEPackageReader::ReadPackageResponseChunk() {
    FEChunk *chunk = FindChild(root, 0x52676b50);
    if (chunk) ReadMessageResponseTags((FETag *)(chunk + 1), FESwap(chunk->size), true);
    return 1;
}
int FEPackageReader::ReadObjectChunk() {
    FEChunk *chunk = FindChild(root, 0xcc6a624f);
    if (!chunk) return 0;
    FEChunk *current = chunk + 1;
    FEChunk *end = (FEChunk *)((char *)chunk + FESwap(chunk->size) + 8);
    if (!current || !end) return 0;
    while (FESwap(current->id) != 0xea624f46 && current < end) {
        if (FESwap(current->id) == 0x6e747542) {
            buttonCount = FESwap(((unsigned long *)(current + 1))[0]);
            package->buttonMap.SetCount(buttonCount);
        }
        current = (FEChunk *)((char *)current + FESwap(current->size) + 8);
    }
    while (current < end) {
        FEChunk *tag = current + 1;
        FEChunk *tagEnd = (FEChunk *)((char *)current + FESwap(current->size) + 8);
        object = 0;
        parentGroup = 0;
        while (tag < tagEnd) {
            switch (FESwap(tag->id)) {
            case 0x446a624f:
                if (!ReadObjectTags((FETag *)(tag + 1), FESwap(tag->size))) return 0;
                break;
            case 0x70726353:
                ReadScriptTags((FETag *)(tag + 1), FESwap(tag->size));
                break;
            case 0x5267734d:
                ReadMessageResponseTags((FETag *)(tag + 1), FESwap(tag->size), false);
                break;
            }
            tag = (FEChunk *)((char *)tag + FESwap(tag->size) + 8);
        }
        if (object) {
            if (object->type == 4) ((FEListBox *)object)->RecalculateCummulative();
            else if (object->type == 6) ((FECodeListBox *)object)->FillAllCells();
            FEScript *script;
            for (script = (FEScript *)object->scriptList.head; script; script = (FEScript *)script->next) {
                if (script->unknown_18) script->unknown_18 = (unsigned long)object->FindScript(script->unknown_18);
            }
            script = object->FindScript(0x1744b3);
            object->SetCurrentScript(script);
            script->time = 0;
            if (!skipInterp) {
                for (unsigned char track = 0; track < (unsigned long)script->trackCount; track++) FEKeyInterp(script, track, 0, object);
            }
            if (parentGroup) parentGroup->childList.AddTail(object);
            else package->objectList.AddTail(object);
        }
        current = (FEChunk *)((char *)current + FESwap(current->size) + 8);
    }
    return 1;
}
FEObject *FEPackageReader::CreateObject(unsigned long type) {
    FEObject *created;
    switch (type) {
    case 2:
        created = new ("source/FEngine/FEPackageReader.cpp:469") FEString(type);
        break;
    case 4:
        created = new ("source/FEngine/FEPackageReader.cpp:473") FEListBox;
        break;
    case 6:
        created = new ("source/FEngine/FEPackageReader.cpp:477") FECodeListBox;
        ((FECodeListBox *)created)->game = game;
        break;
    case 5:
        created = new ("source/FEngine/FEPackageReader.cpp:482") FEGroup(type);
        break;
    case 1:
        created = new ("source/FEngine/FEPackageReader.cpp:486") FEImage(type);
        break;
    case 7:
        created = new ("source/FEngine/FEPackageReader.cpp:490") FEMovie(type);
        break;
    case 9:
        created = new ("source/FEngine/FEPackageReader.cpp:494") FEColoredImage(type);
        break;
    case 10:
        created = new ("source/FEngine/FEPackageReader.cpp:498") FEAnimImage(type);
        break;
    case 11:
        created = new ("source/FEngine/FEPackageReader.cpp:502") FESimpleImage(type);
        break;
    case 0:
        return 0;
    default:
        created = new ("source/FEngine/FEPackageReader.cpp:509") FEObject;
        break;
    }
    created->type = type;
    created->SetDataSize(GetTypeSize(type));
    return created;
}
