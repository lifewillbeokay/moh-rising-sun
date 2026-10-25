#include "ScriptRuntime.h"
extern BSObject *g_pBSObjectListStart, *g_pBSObjectListEnd;
void AddBSObjectToList(BSObject *object) {
    if (!g_pBSObjectListStart) g_pBSObjectListStart = object;
    else g_pBSObjectListEnd->next = object;
    g_pBSObjectListEnd = object;
    object->next = 0;
}
void RemoveBSObjectFromList(BSObject *object) {
    BSObject *last = 0;
    BSObject **link = &g_pBSObjectListStart;
    BSObject *current = g_pBSObjectListStart;
    while (current) {
        if (current == object) *link = current->next;
        else last = current;
        link = &current->next;
        current = current->next;
    }
    g_pBSObjectListEnd = last;
}
