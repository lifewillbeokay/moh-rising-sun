// FEMessageResponse.cpp: a message's list of responses, and its pooled allocation.
#include "FEMessageResponse.h"
#include <string.h>

typedef FEPoolNode<FEMessageResponse, 64> FEMessageResponsePoolNode;
inline void *operator new[](unsigned int size, const char *label) { return DWI_alloc(label, size, 256); }

FEResponse::~FEResponse() { ReleaseParam(); }
FEResponse &FEResponse::operator=(FEResponse &other) {
    ReleaseParam();
    id = other.id;
    if (HasStringParam()) SetParam(other.param);
    else param = other.param;
    value = other.value;
    return *this;
}
void FEResponse::SetParam(const char *text) {
    ReleaseParam();
    if (text) {
        char *copy = (char *)DWI_alloc("source/FEngine/FEMessageResponse.cpp:40", strlen(text) + 1, 256);
        strcpy(copy, text);
        param = copy;
    }
}
void FEResponse::ReleaseParam() {
    if (HasStringParam() && param) delete[] param;
    param = 0;
}
FEMessageResponse::~FEMessageResponse() { PurgeResponses(); }
void *FEMessageResponse::operator new(unsigned int) {
    FEMinList &pool = NodePool;
    FEMessageResponsePoolNode *node;
    for (node = (FEMessageResponsePoolNode *)pool.head; node; node = (FEMessageResponsePoolNode *)node->next) {
        if (node->freeList.count) break;
    }
    if (!node) {
        node = new ("source/FEngine/ObjectPool.h:87") FEMessageResponsePoolNode;
        pool.AddNode(0, node);
    }
    FEMessageResponse *object = (FEMessageResponse *)node->freeList.RemHead();
    node->used++;
    object->message = 0;
    object->count = 0;
    object->responses = 0;
    return object;
}
void FEMessageResponse::operator delete(void *p) {
    ((FEMessageResponse *)p)->~FEMessageResponse();
    FEMinList &pool = NodePool;
    for (FEMessageResponsePoolNode *node = (FEMessageResponsePoolNode *)pool.head; node; node = (FEMessageResponsePoolNode *)node->next) {
        if (node->Contains(p)) {
            node->freeList.AddTail((FEMessageResponse *)p);
            if (--node->used == 0) {
                pool.RemNode(node);
                delete node;
            }
            return;
        }
    }
}
void FEMessageResponse::PurgeResponses() {
    if (responses) delete[] responses;
    responses = 0;
    count = 0;
}
void FEMessageResponse::SetCount(unsigned long newCount) {
    if (newCount != count) {
        if (newCount == 0) {
            PurgeResponses();
        } else {
            FEResponse *list = new ("source/FEngine/FEMessageResponse.cpp:132") FEResponse[newCount];
            unsigned long keep = count;
            if (keep > newCount) keep = newCount;
            for (unsigned long i = 0; i < keep; i++) list[i] = responses[i];
            if (responses) delete[] responses;
            responses = list;
            count = newCount;
        }
    }
}
long FEMessageResponse::FindResponse(unsigned long id) const {
    for (unsigned long i = 0; i < count; i++) {
        if (responses[i].id == id) return i;
    }
    return -1;
}
