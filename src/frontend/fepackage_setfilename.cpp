// FEPackage.cpp: a package's objects, resources and library references.
#include "FEngObjects.h"
#include "FEMessageResponse.h"
#include <string.h>

void FEPackage::SetFilename(const char *name) {
    if (fileName) delete[] fileName;
    fileName = 0;
    if (name) {
        fileName = new ("source/FEngine/FEPackage.cpp:100") char[strlen(name) + 1];
        strcpy(fileName, name);
    }
}
int FEPackage::Startup(FEGameInterface *game) {
    int result = 1;
    if (!game->LoadResources(this, resourceCount, resourceRequests)) result = 0;
    ConnectObjectResources();
    return result;
}
void FEPackage::Shutdown(FEGameInterface *game) {
    if (game) game->UnloadResources(this, resourceCount, resourceRequests);
}
FEMessageResponse *FEPackage::FindResponse(unsigned long message) {
    FEMessageResponse *response = (FEMessageResponse *)responseList.head;
    while (response && response->message != message) response = (FEMessageResponse *)response->next;
    return response;
}
int FEPackage::ForAllChildren(FEGroup *group, FEObjectCallback &callback) {
    for (FEObject *object = (FEObject *)group->childList.head; object; object = (FEObject *)object->next) {
        if (!callback.Callback(object)) return 0;
        if (object->type == 5 && !ForAllChildren((FEGroup *)object, callback)) return 0;
    }
    return 1;
}
int FEPackage::ForAllObjects(FEObjectCallback &callback) {
    for (FEObject *object = (FEObject *)objectList.head; object; object = (FEObject *)object->next) {
        if (!callback.Callback(object)) return 0;
        if (object->type == 5 && !ForAllChildren((FEGroup *)object, callback)) return 0;
    }
    return 1;
}
FEObject *FEPackage::FindObjectByName(const char *name) { return FindObjectByHash(FEHashUpper(name)); }
FEObject *FEPackage::FindObjectByHash(unsigned long hash) {
    FEFindByHash find;
    find.hash = hash;
    find.found = 0;
    ForAllObjects(find);
    return find.found;
}
FEObject *FEPackage::FindObjectByGUID(unsigned long guid) {
    FEFindByGUID find;
    find.guid = guid;
    find.found = 0;
    ForAllObjects(find);
    return find.found;
}
