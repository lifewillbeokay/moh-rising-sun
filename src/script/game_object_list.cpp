// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "BSGameObject.h"
void AddObjectToList(BSGO_Basic *object, BSGO_Basic **head) {
    if (*head)
        (*head)->previous = object;
    object->next = *head;
    object->previous = 0;
    *head = object;
}
void RemoveObjectFromList(BSGO_Basic *object, BSGO_Basic **head) {
    if (object->previous)
        object->previous->next = object->next;
    if (object->next)
        object->next->previous = object->previous;
    if (*head == object)
        *head = object->next;
}
