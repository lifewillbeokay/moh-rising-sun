// CFengInterface's render contexts, object dispatch and pad mask; see docs/Frontend.md.
#include "FengInterface.h"
#include "Shell.h"
#include "FEColor.h"
void CFengInterface::GenerateRenderContext(unsigned short index, FEObject *object) {
    GenerateXformMatrix(&contexts[index].transform, (FEObjData *)object->data, object->renderContext);
    contexts[index].color = *(FEColor *)object->data;
    contexts[index].color *= contexts[object->renderContext].color;
}
int CFengInterface::GetContextTransform(unsigned short index, FEMatrix4 &matrix) {
    EAGL::Transform *transform = &contexts[index].transform;
    matrix = *(FEMatrix4 *)transform;
    return 1;
}
void CFengInterface::GetViewTransformation(FEMatrix4 *matrix) { matrix->Identify(); }
void CFengInterface::RenderObject(FEObject *object) {
    switch ((FEObjectKind)object->type) {
    case FEObjectKindImage:
        RenderImage((FEImage *)object);
        break;
    case FEObjectKindSimpleImage:
        Render((FESimpleImage *)object);
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
struct SPadFlag {
    unsigned short unknown_0;
    unsigned short key;
    unsigned long flag;
};
extern SPadFlag padFlags[];
unsigned long CFengInterface::GetJoyPadMask(unsigned char pad) {
    if (g_inputMgr.badControllerSet) {
        return 0;
    }
    unsigned long mask = 0;
    if (!g_inputMgr.IsReady(pad)) {
        return 0;
    }
    for (int i = 0; i < 9; i++) {
        if (g_inputMgr.GetValue(pad, padFlags[i].key)) {
            mask = padFlags[i].flag;
            break;
        }
    }
    int x = g_inputMgr.GetValue(pad, 129);
    int y = g_inputMgr.GetValue(pad, 130);
    if (mask == 0) {
        if (x < 0 && lastStickX >= 0) {
            mask |= 4;
        } else if (x > 0 && lastStickX <= 0) {
            mask |= 8;
        } else if (y < 0 && lastStickY >= 0) {
            mask |= 1;
        } else if (y > 0 && lastStickY <= 0) {
            mask |= 2;
        }
    }
    lastStickX = x;
    lastStickY = y;
    return mask;
}
