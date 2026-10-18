// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneObjects.h"

void BIFunc_InitializeAnimatedTexture(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int crc = *(*stack - (count - 1));
    int mode = *(*stack - (count - 2));
    float rate = *(float *)(*stack - (count - 3));
    if (crc) {
        AnimShape::EState state = AnimShape::StateDefault;
        switch (mode) {
        case 0:
            break;
        case 1:
            state = AnimShape::StateL;
            break;
        case 2:
            state = AnimShape::Statel;
            break;
        case 3:
            state = AnimShape::StateO;
            break;
        case 4:
            state = AnimShape::Stateo;
            break;
        case 5:
            state = AnimShape::StateP;
            break;
        case 6:
            state = AnimShape::Statep;
            break;
        default:
            state = AnimShape::StateDefault;
            break;
        }
        AnimShape::InitByCRC(crc, state, rate);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_SetAnimatedTextureState(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int crc = *(*stack - (count - 1));
    int mode = *(*stack - (count - 2));
    if (crc) {
        AnimShape::EState state = AnimShape::StateDefault;
        switch (mode) {
        case 0:
            break;
        case 1:
            state = AnimShape::StateL;
            break;
        case 2:
            state = AnimShape::Statel;
            break;
        case 3:
            state = AnimShape::StateO;
            break;
        case 4:
            state = AnimShape::Stateo;
            break;
        case 5:
            state = AnimShape::StateP;
            break;
        case 6:
            state = AnimShape::Statep;
            break;
        default:
            state = AnimShape::StateDefault;
            break;
        }
        AnimShape::SetStateByCRC(crc, state);
    }
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
