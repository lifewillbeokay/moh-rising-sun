// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
void FEngine::RenderObject(FEObject *object, FEMatrix4 &view, unsigned short context) {
    FEObjData *data = (FEObjData *)object->data;
    if (data->visible) {
        FEVector3 corner;
        corner = data->pivot;
        FEVector3 projected(0.0f, 0.0f, 0.0f);
        corner.x += data->position.x;
        corner.y += data->position.y;
        corner.z += data->position.z;
        FEMultMatrix(&projected, &view, &corner);
        object->renderContext = context;
        if (!(projected.z <= 0.0f)) {
            union { float value; int bits; } depth;
            depth.value = projected.z;
            sorter.entries[sorter.count].object = object;
            sorter.entries[sorter.count].depthBits = depth.bits;
            ++sorter.count;
        }
    }
}
#endif
