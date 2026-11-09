// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic reference.
// Part of the original staticmesh.cpp.
#include "StaticMesh.h"

unsigned int GetFrameCount();
#include "AnimShape.h"
struct AnimShapeList {
    AnimShape *begin;
    AnimShape *end;
};
class MaterialAnim {
public:
    void Update(float);
    unsigned char unknown00[16];
    unsigned int updateFrame; // +0x10
};

void CStaticMesh::Update(float elapsed) {
    if (animShapes) {
        unsigned int frame = GetFrameCount();
        for (AnimShape *shape = animShapes->begin; shape != animShapes->end; shape++) {
            if (shape->updateFrame != frame) {
                shape->Update(elapsed);
                shape->updateFrame = frame;
            }
        }
    }
    if (mesh->materialAnims) {
        unsigned int frame = GetFrameCount();
        for (int i = 0; i < mesh->materialAnims->count; i++) {
            MaterialAnim *anim = mesh->materialAnims->anims[i];
            if (anim->updateFrame != frame) {
                anim->Update(elapsed);
                anim->updateFrame = frame;
            }
        }
    }
}

void CStaticMesh::RebindAnimatedTextures() {
    if (animShapes) {
        for (AnimShape *shape = animShapes->begin; shape != animShapes->end; shape++)
            shape->RebindTARs();
    }
}

