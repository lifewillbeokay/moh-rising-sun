// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "SceneObjects.h"

void BIFunc_CreateChild(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int result = 0;
    int id = *(*stack - (count - 1));
    int type = *(*stack - (count - 2));
    char *asset = (char *)*(*stack - (count - 3));
    switch (type) {
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        result = (int)CreateObject(g_pBSObject->nativeObject, id, type, AssetCRC(asset), 0);
        break;
    case 3: {
        int *child = CreateObject(g_pBSObject->nativeObject, id, type, AssetCRC(asset), 0);
        if (child)
            result = child[2];
        break;
    }
    default: {
        int *child = CreateObject(g_pBSObject->nativeObject, id, type, AssetCRC(asset), 0);
        if (child)
            result = child[2];
        break;
    }
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
