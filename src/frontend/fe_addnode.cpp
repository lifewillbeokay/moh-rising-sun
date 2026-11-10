// FEMinList and FEList: FEng's doubly linked lists.
#include "FEngLists.h"

void FEMinList::AddNode(FEMinNode *after, FEMinNode *node) {
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
    count++;
}
FEMinNode *FEMinList::RemNode(FEMinNode *node) {
    if (node) {
    if (node == head) head = node->next;
    if (node == tail) tail = node->prev;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    node->prev = node->next = (FEMinNode *)0xabadcafe;
    count--;
    }
    return node;
}
FEMinNode *FEMinList::RemHead() {
    FEMinNode *node = head;
    if (node) RemNode(node);
    return node;
}
FEMinNode *FEMinList::FindNode(unsigned long index) const {
    FEMinNode *node = head;
    unsigned long i = 0;
    while (node && i != index) {
        node = node->next;
        i++;
    }
    return node;
}
FENode *FEList::FindNode(const char *name, FENode *start) const {
    unsigned long hash = FEHashUpper(name);
    FENode *found = 0;
    for (; start; start = (FENode *)start->next) {
        if (start->name) {
            if (hash == start->hash && FEStricmp(start->name, name) == 0) {
                found = start;
                break;
            }
        } else if (!name) {
            found = start;
            break;
        }
    }
    return found;
}
FENode *FEList::FindNode(const char *name) const { return FindNode(name, (FENode *)head); }
