// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "FEScript.h"
#include "FEMessageResponse.h"
#include <string.h>
FEObject::FEObject() : hash(0), name(0), flags(0), renderContext(0),
    resourceData(0), resourceHandle(0), data(0), dataSize(0) {
    guid = FEngine::SysGUID++;
}
FEObject::FEObject(const FEObject &other, bool referenceScripts)
    : hash(0), name(0), flags(0), resourceData(0), resourceHandle(0), data(0) {
    guid = FEngine::SysGUID++;
    SetDataSize(other.dataSize);
    memset(data, 0, dataSize);
    type = other.type; flags = other.flags;
    renderContext = other.renderContext; resourceIndex = other.resourceIndex;
    resourceData = other.resourceData;
    SetName(other.name);
    for (FEMessageResponse *node = (FEMessageResponse *)other.responseList.head; node;
         node = (FEMessageResponse *)node->next) {
        FEMessageResponse *copy = new FEMessageResponse;
        unsigned long count = node->count;
        copy->SetCount(count); copy->message = node->message;
        for (unsigned long i = 0; i < count; ++i) copy->responses[i] = node->responses[i];
        responseList.AddNode(responseList.tail, copy);
    }
    for (FEScript *node = (FEScript *)other.scriptList.head; node; node = (FEScript *)node->next)
        scriptList.AddNode(scriptList.tail, new FEScript(*node, referenceScripts));
    SetCurrentScript(FindScript(other.currentScript->hash));
    FEScript *copy = (FEScript *)scriptList.head;
    for (FEScript *node = (FEScript *)other.scriptList.head; node; node = (FEScript *)node->next) {
        if (node->unknown_18) copy->unknown_18 = (unsigned long)FindScript(((FEScript *)node->unknown_18)->hash);
        copy = (FEScript *)copy->next;
    }
}
#endif
