// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "ShellManager.h"
#include "FEngObjects.h"
#include "FEngRender.h"
#include <string.h>
#include <stdio.h>
#include "FEngElements.h"
#ifdef DECOMP_COMPARE
void ShellManager::GenerateXformMatrix(EAGL::Transform *matrix, FEObjData *data, unsigned short parent) {
    COORD4 translation;
    memset(&translation, 0, sizeof(translation));
    COORD4 rotation;
    rotation.x = data->rotation.x; rotation.y = data->rotation.y;
    rotation.z = data->rotation.z; rotation.w = data->rotation.w;
    translation = rotation;
    memset(&translation, 0, sizeof(translation));
    translation.w = 1.0f;
    matrix->BuildScale(data->scale.x, data->scale.y, data->scale.z, 1.0f);
    matrix->PrependQuatTrans(&rotation, &translation);
    matrix->AppendTranslate(data->position.x, data->position.y, data->position.z);
    if (parent) matrix->PostMult(contexts[parent].transform);
}
void ShellManager::GenerateRenderContext(unsigned short index, FEObject *object) {
    GenerateXformMatrix(&contexts[index].transform, (FEObjData *)object->data, object->renderContext);
    contexts[index].color = *(FEColor *)object->data;
    contexts[index].color *= contexts[object->renderContext].color;
}
int ShellManager::GetContextTransform(unsigned short index, FEMatrix4 &matrix) {
    matrix = *(FEMatrix4 *)&contexts[index].transform;
    return 1;
}
void ShellManager::RenderObjectList(FEObjectListEntry *objects, unsigned long count) {
    if (count) {
        FEObjectListEntry *it = objects + count;
        do { --it; RenderObject((FEObject *)it->object); } while (it != objects);
    }
}

void ShellManager::RenderObject(FEObject *object) {
    switch ((FEObjectKind)object->type) {
    case FEObjectKindImage:
        RenderImage((FEImage *)object);
        break;
    case FEObjectKindSimpleImage:
        RenderSimpleImage((FESimpleImage *)object);
        break;
    case FEObjectKindColoredImage:
        printf("RenderObject: colored image NOT IMPLEMENTED\n");
        break;
    case FEObjectKindString:
        RenderString((FEString *)object);
        break;
    case FEObjectKindModel:
        printf("RenderObject: model NOT IMPLEMENTED\n");
        break;
    case FEObjectKindList:
        printf("RenderObject: list NOT IMPLEMENTED\n");
        break;
    case FEObjectKindCodeList:
        printf("RenderObject: codelist NOT IMPLEMENTED\n");
        break;
    case FEObjectKindMovie:
        printf("RenderObject: movie NOT IMPLEMENTED\n");
        break;
    case FEObjectKindEffect:
        printf("RenderObject: effect NOT IMPLEMENTED\n");
        break;
    case FEObjectKindNone:
    case FEObjectKindGroup:
    case FEObjectKindCount:
    default:
        printf("RenderObject: unexpected type identifier (%d)!\n", object->type);
        break;
    }
}
#endif
