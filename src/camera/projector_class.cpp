// AI-assisted reconstruction of the projector class's setup and shutdown; see docs/Rendering.md.
#include "profile.h"
#include <map>
#include "Projector.h"
#include "ProjectorSystemDef.h"
#include "Toilet.h"
extern "C" void *DWI_alloc(const char *, unsigned int, unsigned int);
typedef _STL::map<unsigned int, CProjectorSystemDef> ProjectorDefMap;
extern ProjectorDefMap g_ProjectorSystemsDefs;
extern CMatrix g_MOH3TextureProjector;
extern int g_bReady;
void CProjector::InitClass(CScene *scene) {
    m_pScene = scene;
    ProjectorList &list = g_ProjectorList;
    list.capacity = 16;
    list.storage = (CProjectorPoolNode *)DWI_alloc("source/system/freepool.h:38", 9984, 1024);
    list.active = 0;
    list.free = list.storage;
    list.count = 0;
    if (list.capacity > 0) {
        list.storage[list.capacity - 1].next = 0;
        for (int i = 0; i < list.capacity - 1; i++)
            list.free[i].next = &list.free[i + 1];
    }
    g_MOH3TextureProjector.GetSlot(0);
    g_MOH3ProjectionTexture = 0;
    LevelFileContentsStruct_ *resource = TLT_FindNextResourceByType((TLTResourceID)16, 0);
    while (resource) {
        CProjectorSystemDef def(resource);
        unsigned int key = def.resource->data[0]->values[0];
        g_ProjectorSystemsDefs[key] = def;
        resource = TLT_FindNextResourceByType((TLTResourceID)16, resource);
    }
    g_bReady = 1;
}
void CProjector::ShutdownClass() {
    g_bReady = 0;
    g_ProjectorSystemsDefs.clear();
    ProjectorList &list = g_ProjectorList;
    DWI_free(list.storage);
    g_ProjectorList.storage = 0;
    list.capacity = 0;
    list.count = 0;
    list.active = 0;
    list.free = 0;
}
