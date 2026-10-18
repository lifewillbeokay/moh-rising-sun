// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "SceneNode.h"
#include "WorldObject.h"

// The original call sets CR1 as for a variadic callee; this declaration reproduces it.
extern "C" void *memset(void *, int, ...);

void BIFunc_LoadCompartment(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int first = *(*stack - (count - 1));
    int second = *(*stack - (count - 2));
    int third = *(*stack - (count - 3));
    int fourth = *(*stack - (count - 4));
    CWorldObject::SAcquireSet request;
    CWorldObject::SAcquireSet set;
    memset(&set, 0, sizeof(set));
    set.compartments[0] = (short)first;
    set.compartments[1] = (short)second;
    set.compartments[2] = (short)third;
    set.compartments[3] = (short)fourth;
    request = set;
    g_scene.world->Acquire(request, true, false);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
