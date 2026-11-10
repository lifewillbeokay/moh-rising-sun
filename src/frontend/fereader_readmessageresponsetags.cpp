// FEPackageReader.cpp: object tags.
#include "FEPackageReader.h"
#include "FEngine.h"
#include "FEngStrings.h"
#include "FEngElements.h"
#include "FEScript.h"
#include "FEMessageResponse.h"

int FEPackageReader::ReadMessageResponseTags(FETag *tag, unsigned long size, bool forPackage) {
    FETag *end = (FETag *)((char *)tag + size);
    FEMessageResponse *message = 0;
    FEResponse *response = 0;
    if (tag >= end) return 1;
    long offset = -12;
    do {
        switch (FESwapShort(tag->id)) {
        case 0x694d:
        {
            message = 0;
            long id = FETagLong(tag, 0);
            if (!forPackage && isReference) message = object->FindResponse(id);
            if (!message) {
                message = new FEMessageResponse;
                message->message = id;
                if (forPackage) package->responseList.AddTail(message);
                else object->responseList.AddTail(message);
            } else {
                message->PurgeResponses();
            }
            offset = -12;
            break;
        }
        case 0x434d:
            message->SetCount(FETagLong(tag, 0));
            break;
        case 0x6952:
            offset += 12;
            response = (FEResponse *)((char *)message->responses + offset);
            response->id = FETagLong(tag, 0);
            break;
        case 0x7552:
            response->param = (char *)FETagLong(tag, 0);
            break;
        case 0x7352:
            response->SetParam((const char *)(tag + 1));
            break;
        case 0x7452:
            response->value = FETagLong(tag, 0);
            break;
        }
        tag = (FETag *)((char *)tag + (FESwapShort(tag->size) + 4));
    } while (tag < end);
    return 1;
}
