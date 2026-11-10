// AI-assisted reconstruction of the projector's creation from its definition; see docs/Rendering.md.
#include "profile.h"
#include <map>
#include "Projector.h"
#include "ProjectorSystemDef.h"
typedef _STL::map<unsigned int, CProjectorSystemDef> ProjectorDefMap;
extern ProjectorDefMap g_ProjectorSystemsDefs;
CProjector *CProjector::Create(unsigned int id, bool addToScene) {
    CProjector *projector = 0;
    ProjectorDefMap::iterator found = g_ProjectorSystemsDefs.find(id);
    if (found != g_ProjectorSystemsDefs.end()) {
        projector = Alloc();
        if (projector) {
            new (projector) CProjector(g_ProjectorSystemsDefs[id]);
            if (addToScene)
                g_scene.Add(*projector, 0);
        }
    }
    return projector;
}
