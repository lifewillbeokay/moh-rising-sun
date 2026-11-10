// CFengInterface's UI object registry, texture and font loading and transforms; see docs/Frontend.md.
#include "FengInterface.h"
char *TLT_LoadFileFromBigFile(char *, const char *, int *);
void CFengInterface::RegisterUIObject(CUIObject *object) {
    printf(" Registering UI Object\n");
    uiObjects.insert(object);
}
void CFengInterface::ReplaceFilenameExtension(char *name, char *extension) {
    char *dot = strchr(name, '.');
    if (dot) {
        *dot = 0;
    }
    sprintf(name, "%s.%s", name, extension);
}
int CFengInterface::LoadTexture(FEResourceRequest *request, char *file) {
    ReplaceFilenameExtension(file, "gsh");
    char *data = TLT_LoadFileFromBigFile(bigFile, file, 0);
    request->handle = new ("source/UserInterface/FengInterface.cpp:519") CSprite((ShapeFile *)data);
    char *copy = (char *)DWI_alloc("source/UserInterface/FengInterface.cpp:523", strlen(file) + 1, 1024);
    strcpy(copy, file);
    request->data = copy;
    return 1;
}
int CFengInterface::LoadFont(FEResourceRequest *request, char *file) {
    ReplaceFilenameExtension(file, "gfn");
    char *data = TLT_LoadFileFromBigFile(bigFile, file, 0);
    request->handle = new ("source/UserInterface/FengInterface.cpp:547") CFont(data);
    request->data = 0;
    return 1;
}
void CFengInterface::GenerateXformMatrix(EAGL::Transform *transform, FEObjData *data, unsigned short parent) {
    transform->BuildScale(data->scale.x, data->scale.y, data->scale.z, 1.0f);
    transform->AppendTranslate(data->position.x, data->position.y, data->position.z);
    if (parent) {
        transform->PostMult(contexts[parent].transform);
    }
}
