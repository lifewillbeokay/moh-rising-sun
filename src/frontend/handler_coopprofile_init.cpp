// HandlerCoopProfile's setup; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
static inline void SetVector(FEVector3 &v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}
int HandlerCoopProfile::Init(const char *name) {
    g_Shell.inCoop = 1;
    selection = -1;
    FEObject *group = package->FindObjectByName("Quadrant Group");
    if (group) {
        FEVector3 top;
        SetVector(top, 0.0f, -120.0f, 100.0f);
        FEVector3 bottom;
        SetVector(bottom, 0.0f, 120.0f, 100.0f);
        int mask = package->controlMask;
        switch (mask) {
        case 1:
            group->SetPosition(top, false);
            m_player1_profile_index = -1;
            break;
        case 2:
            group->SetPosition(bottom, false);
            m_player2_profile_index = -1;
            break;
        }
        controlMask = mask;
        m_create_player1 = m_create_player2 = 0;
    }
    return PackageHandler::Init(name);
}
