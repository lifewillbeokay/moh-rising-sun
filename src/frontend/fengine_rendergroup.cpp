// FEngine: rendering a package's objects.
#include "FEngine.h"
#include "ShellManager.h"

void FEngine::RenderGroup(FEGroup *group, FEMatrix4 &parent, FEMatrix4 &viewMatrix, unsigned short parentContext) {
    FEObjData *data = (FEObjData *)group->data;
    FEMatrix4 local;
    FEMatrix4 world;
    FEMatrix4 combined;
    FEVector3 corner;
    FEVector3 offset;
    corner.x = corner.y = corner.z = 0.0f;
    offset.x = offset.y = offset.z = 0.0f;
    if (data->visible && (executing || (long)group->flags >= 0)) {
        data->rotation.GetMatrix(&local);
        const FEVector3 &pivot = data->pivot;
        offset.x = pivot.x;
        offset.y = pivot.y;
        offset.z = pivot.z;
        offset.x = -offset.x;
        offset.y = -offset.y;
        offset.z = -offset.z;
        FEMultMatrix(&corner, &local, &offset);
        local.m[3][0] = corner.x + data->pivot.x + data->position.x;
        local.m[3][1] = corner.y + data->pivot.y + data->position.y;
        local.m[3][2] = corner.z + data->pivot.z + data->position.z;
        FEMultMatrix(&world, &local, &parent);
        FEMultMatrix(&combined, &world, &viewMatrix);
        unsigned short context = ++renderContextCount;
        group->renderContext = parentContext;
        game->GenerateRenderContext(context, group);
        for (FEObject *child = (FEObject *)group->childList.head; child; child = (FEObject *)child->next) {
            if (child->type == 5) RenderGroup((FEGroup *)child, world, viewMatrix, context);
            else RenderObject(child, combined, context);
        }
    }
}
