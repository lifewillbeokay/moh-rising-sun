// A fragment of the original glare code: the glare manager's pool and resource table. Names come from the symbol table;
// member names are inferred from the code.
#include "GlareManager.h"

// The pool's allocation is tagged with this line of the original's header.
inline void GlarePool::Init(int size) {
    capacity = size;
    storage = (CGlare *)DWI_alloc("source/system/freepool.h:38", size * sizeof(CGlare), 1024);
    active = 0;
    free = storage;
    count = 0;
    LinkFree();
}

CGlareManager::CGlareManager() : enabled(0), unknown04(1), resourceCount(0), resources(0) {}

void CGlareManager::Init() {
    enabled = 1;
    LevelFileContentsStruct_ *r = TLT_FindNextResourceByType((TLTResourceID)15, 0);
    while (r) {
        resourceCount++;
        r = TLT_FindNextResourceByType((TLTResourceID)16, r);
    }
    if (resourceCount) {
        resources = new CGlareResource[resourceCount];
        r = TLT_FindNextResourceByType((TLTResourceID)15, 0);
        int i = 0;
        while (r) {
            resources[i++].Init(r);
            r = TLT_FindNextResourceByType((TLTResourceID)16, r);
        }
        pool.Init(64);
    }
}

void CGlareManager::Reset() { pool.Reset(); }

void CGlareManager::Shutdown() {
    pool.Shutdown();
    if (resources)
        delete resources;
    resources = 0;
    enabled = 0;
}

CGlare *CGlareManager::Create(int id) {
    CGlareResource *resource = Find(id);
    if (!resource)
        return 0;
    CGlare *glare = pool.Alloc();
    if (glare)
        glare = new (glare) CGlare(*resource);
    return glare;
}

CGlareResource *CGlareManager::Find(int id) {
    CGlareResource *found = 0;
    for (int i = 0; i < resourceCount; i++) {
        if (id == resources[i].GetId()) {
            found = &resources[i];
            break;
        }
    }
    return found;
}

CGlareResource::CGlareResource() { resource = 0; }
