// FEPackageReader.cpp: object tags.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEngElements.h"
#include "FEScript.h"
#include "FEMessageResponse.h"

int FEPackageReader::ReadObjectTags(FETag *tag, unsigned long size) {
    FETag *end = (FETag *)((char *)tag + size);
    isReference = 0;
    while (tag < end) {
        switch (FESwapShort(tag->id)) {
        case 0x744f:
            object = CreateObject(FETagLong(tag, 0));
            break;
        case 0x6e4f:
            if (isLibrary) object->SetName((const char *)(tag + 1));
            break;
        case 0x504f: {
            object->guid = FETagLong(tag, 0);
            object->hash = FETagLong(tag, 1);
            object->flags = FETagLong(tag, 2);
            object->resourceIndex = FETagLong(tag, 3);
            if (object->flags & 0x100000) {
                int found = FindReferencedObject(object->guid, &referencedObject, &referencedPackage);
                if (!found) {
                    delete object;
                    object = 0;
                    return 0;
                }
                isReference = 1;
                object->flags |= referencedObject->flags;
                if (object->resourceIndex == 0xffff) {
                    object->resourceData = referencedObject->resourceData;
                    object->resourceHandle = referencedObject->resourceHandle;
                }
                FEObject *clone;
                if (referencedObject->type != 5) clone = referencedObject->Clone(true);
                else clone = new FEGroup(*(FEGroup *)referencedObject, false, true);
                clone->SetName(object->name);
                clone->guid = object->guid;
                clone->hash = object->hash;
                clone->flags = object->flags;
                clone->resourceIndex = object->resourceIndex;
                clone->resourceData = object->resourceData;
                clone->resourceHandle = object->resourceHandle;
                delete object;
                object = clone;
                for (FEScript *script = (FEScript *)clone->scriptList.head; script; script = (FEScript *)script->next) {
                    if (script->unknown_18) script->unknown_18 = ((FEScript *)script->unknown_18)->hash;
                }
            }
            if (object->flags & 0x10000000) {
                package->buttonMap.buttons[buttonIndex] = object;
                buttonIndex++;
            }
            break;
        }
        case 0x4150:
            if (!parent || parent->guid != FETagLong(tag, 0)) parent = package->FindObjectByGUID(FETagLong(tag, 0));
            parentGroup = (FEGroup *)parent;
            break;
        case 0x4153: {
            unsigned long count = FESwapShort(tag->size) >> 2;
            for (unsigned long i = 0; i < count; i++) {
                ((unsigned long *)object->data)[i] = FETagLong(tag, i);
            }
            break;
        }
        default:
            if (object) {
                switch (object->type) {
                case 2:
                    ProcessStringTag(tag);
                    break;
                case 4:
                    ProcessListBoxTag(tag);
                    break;
                case 6:
                    ProcessCodeListBoxTag(tag);
                    break;
                case 1:
                case 9:
                case 10:
                    ProcessImageTag(tag);
                    break;
                }
            }
            break;
        }
        tag = (FETag *)((char *)tag + (FESwapShort(tag->size) + 4));
    }
    return 1;
}
void FEPackageReader::ProcessStringTag(FETag *tag) {
    FEString *string = (FEString *)object;
    switch (FESwapShort(tag->id)) {
    case 0x6253:
        string->text.SetLength(FETagLong(tag, 0));
        break;
    case 0x7453: {
        string->text = (const short *)(tag + 1);
        short *c = string->text.text;
        while (*c) {
            short value = *c;
            *c = ((value >> 8) & 0xff) | (value << 8);
            c++;
        }
        break;
    }
    case 0x6a53:
        string->unknown_68 = FETagLong(tag, 0);
        break;
    case 0x6c53:
        string->unknown_6c = FETagLong(tag, 0);
        break;
    case 0x7753:
        string->unknown_70 = FETagLong(tag, 0);
        break;
    case 0x4c53:
        if (isLibrary) string->SetLabel((const char *)(tag + 1));
        break;
    case 0x4853:
        string->labelHash = FETagLong(tag, 0);
        string->flags |= 0x400000;
        if (FEString::pLabelCallback) FEString::pLabelCallback->LabelRead(string);
        break;
    }
}
void FEPackageReader::ProcessImageTag(FETag *tag) {
    FEImage *image = (FEImage *)object;
    if (FESwapShort(tag->id) == 0x6649) image->unknown_58 = FESwap(*(unsigned long *)(tag + 1));
}
