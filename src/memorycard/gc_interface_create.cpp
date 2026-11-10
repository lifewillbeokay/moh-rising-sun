// AI-assisted reconstruction of the memory-card library's GameCube interface: creating the instance; see docs/Memory.md.
#include "RealmcInterface.h"
namespace Realmc {
Interface *Interface::CreateInstance(const ConfigSettings &settings) {
    SetMemVectors(settings.memAlloc, settings.memFree);
    if (settings.sizeAlloc && settings.sizeFree)
        SetSizeMemVectors(settings.sizeAlloc, settings.sizeFree);
    if (!gInterface)
        gInterface = new GCInterface(settings);
    else
        gInterface->AddRef();
    return gInterface;
}
} // namespace Realmc
