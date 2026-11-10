// FERefList: a linked list that can refer to another list's nodes instead of owning its own.
#include "FEngLists.h"

void FERefList::ReferenceList(FERefList *list) {
    if (list) {
        if (!isReference) {
            for (FEMinNode *node = RemHead(); node; node = RemHead()) delete node;
        }
        head = (FEMinNode *)list;
        isReference = 1;
    } else {
        head = (FEMinNode *)list;
        tail = (FEMinNode *)list;
        isReference = (int)list;
    }
}
void FERefList::AddNode(FEMinNode *after, FEMinNode *node) {
    if (!node) return;
    if (after) {
        FEMinNode *next = after->next;
        node->next = next;
        if (next) next->prev = node;
        node->prev = after;
        after->next = node;
    } else {
        FEMinNode *first = head;
        node->next = first;
        if (first) first->prev = node;
        node->prev = after;
        head = node;
    }
    if (tail == after) tail = node;
}
FEMinNode *FERefList::RemNode(FEMinNode *node) {
    if (node) {
    if (node == head) head = node->next;
    if (node == tail) tail = node->prev;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = node->next = (FEMinNode *)0xabadcafe;
    }
    return node;
}
FEMinNode *FERefList::RemHead() {
    FEMinNode *node = head;
    if (node) RemNode(node);
    return node;
}
int FERefList::GetNumElements() {
    int n = 0;
    for (FEMinNode *node = GetHead(); node; node = node->next) n++;
    return n;
}
