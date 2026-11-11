// AI-assisted GR8E69 reconstruction; see docs/research/feng-packages.md.
#ifdef DECOMP_COMPARE
#include "FEngine.h"
#include "ShellManager.h"
void FEngine::Render() {
    FEMatrix4 world, view;
    world.Identify();
    game->GetViewTransformation(&view);
    for (FEPackage *package = (FEPackage *)packages.head; package; package = (FEPackage *)package->next) {
        renderContextCount = 0;
        game->BeginPackageRendering(package);
        sorter.count = 0;
        for (FEObject *object = (FEObject *)package->objectList.head; object; object = (FEObject *)object->next) {
            if (object->type == 5) RenderGroup((FEGroup *)object, world, view, 0);
            else RenderObject(object, view, 0);
        }
        sorter.SortObjects();
        game->RenderObjectList(sorter.entries, sorter.count);
        game->EndPackageRendering(package);
    }
    unknown_5264 = executing;
}
#endif
