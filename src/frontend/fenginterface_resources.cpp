// CFengInterface's resource loading and package callbacks; see docs/Frontend.md.
#include "FengInterface.h"
char *TLT_LoadFileFromBigFile(char *, const char *, int *);
void CFengInterface::SetBigFilePtr(char *file) { bigFile = file; }
int CFengInterface::LoadResources(FEPackage *, long count, FEResourceRequest *requests) {
    int result = 1;
    for (long i = 0; i < count; i++) {
        char *file = strrchr(requests[i].name, '\\');
        if (file) {
            file++;
        } else {
            file = requests[i].name;
        }
        int loaded = 0;
        switch (requests[i].type) {
        case 1:
            loaded = LoadTexture(&requests[i], file);
            break;
        case 2:
            loaded = LoadFont(&requests[i], file);
            break;
        case 3:
            printf("** Don't know how to load model file '%s'\n", file);
            break;
        case 4:
            printf("** Don't know how to load movie file '%s'\n", file);
            break;
        case 5:
            printf("** Don't know how to load effect file '%s'\n", file);
            break;
        }
        if (!loaded) {
            result = 0;
        }
    }
    return result;
}
int CFengInterface::UnloadResources(FEPackage *, long count, FEResourceRequest *requests) {
    unknown_1870 = 0;
    for (long i = 0; i < count; i++) {
        switch (requests[i].type) {
        case 1:
            delete (CSprite *)requests[i].handle;
            delete[] (char *)requests[i].data;
            requests[i].data = 0;
            requests[i].handle = 0;
            break;
        case 2:
            delete (CFont *)requests[i].handle;
            requests[i].handle = 0;
            break;
        case 3:
            break;
        }
    }
    return 1;
}
int CFengInterface::GetPackageData(const char *name, unsigned char **data, bool &owned) {
    char *file = TLT_LoadFileFromBigFile(bigFile, name, 0);
    *data = (unsigned char *)file;
    owned = false;
    return (int)file;
}
static inline void SetVector(FEVector3 &v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}
void CFengInterface::PackageWasLoaded(FEPackage *package) {
    FEObject *group = package->FindObjectByName("Quadrant Group");
    if (group) {
        FEVector3 upperLeft;
        FEVector3 upperRight;
        FEVector3 lowerLeft;
        FEVector3 lowerRight;
        SetVector(upperLeft, -160.0f, -120.0f, 100.0f);
        SetVector(upperRight, 160.0f, -120.0f, 100.0f);
        SetVector(lowerLeft, -160.0f, 120.0f, 100.0f);
        SetVector(lowerRight, 160.0f, 120.0f, 100.0f);
        switch (package->controlMask) {
        case 1:
            group->SetPosition(upperLeft, false);
            break;
        case 2:
            group->SetPosition(upperRight, false);
            break;
        case 4:
            group->SetPosition(lowerLeft, false);
            break;
        case 8:
            group->SetPosition(lowerRight, false);
            break;
        }
    }
    package->Update(engine, 0);
}
int CFengInterface::PackageWillUnload(FEPackage *) { return 1; }
void CFengInterface::BeginPackageRendering(FEPackage *) {}
void CFengInterface::EndPackageRendering(FEPackage *) {}
