// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "SceneObjects.h"

void BIFunc_DestroyChild(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    ISceneNode *child = (ISceneNode *)*(*stack - (count - 1));
    switch (*(*stack - (count - 2))) {
    case 0:
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 10: case 11:
        child->MarkForDestruction(0);
        break;
    case 7: case 8: case 9:
        ((CAnimObjectInterface *)child)->CleanUp();
        break;
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
