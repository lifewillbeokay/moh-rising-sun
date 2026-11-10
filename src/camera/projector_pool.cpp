// AI-assisted reconstruction of the projector pool; see docs/Rendering.md.
#include "Projector.h"
void *CProjector::operator new(unsigned int, void *p) { return p; }
void CProjector::operator delete(void *) {}
CProjector *CProjector::Alloc() {
    CProjectorPoolNode *projector = g_ProjectorList.free;
    if (projector) {
        g_ProjectorList.free = projector->next;
        projector->next = g_ProjectorList.active;
        g_ProjectorList.active = projector;
        g_ProjectorList.count++;
        return (CProjector *)projector;
    }
    return 0;
}
void CProjector::Release(CProjector *released) {
    CProjectorPoolNode *projector = (CProjectorPoolNode *)released;
    ProjectorList &list = g_ProjectorList;
    if (projector == list.active) {
        list.active = projector->next;
    } else {
        for (CProjectorPoolNode *walk = list.active; walk; walk = walk->next) {
            if (walk->next == projector) {
                walk->next = projector->next;
                break;
            }
        }
    }
    list.count--;
    projector->next = list.free;
    list.free = projector;
}
