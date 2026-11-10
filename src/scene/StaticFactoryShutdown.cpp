// AI-assisted reconstruction from GR8E69, with Medal of Honor: Frontline as a semantic
// reference. Part of the original staticobjfactory.cpp. CAnyStaticMesh's symbol names it;
// its layout, the definition record and the member names are inferred from their accesses.
#include "SceneObjects.h"

class CGameObjectFactory {
  public:
    void Shutdown();
};
class CStaticMesh {
  public:
    unsigned char unknown00[0x18];
    virtual ~CStaticMesh(); // Table pointer at +0x18.
};
// A mesh slot whose type word is negative when empty.
class CAnyStaticMesh {
  public:
    int type;
    unsigned char storage[40];
    CStaticMesh *ptr();
    CAnyStaticMesh() { type = -1; }
    ~CAnyStaticMesh() {
        if (ptr())
            ptr()->~CStaticMesh();
        type = -1;
    }
};
// A 112-byte sorted template record; only the fields below are established.
class StaticObjectDefinition {
  public:
    CAnyStaticMesh mesh;
    int id;           // +0x2c
    unsigned char unknown30[4];
    MotionData *motion; // +0x34
    unsigned char unknown38[8];
    int variant;      // +0x40
    unsigned char unknown44[112 - 0x44];
};
class CStaticObjectFactory {
  public:
    static CGameObjectFactory m_StaticObjectFactory;
    static CGameObjectFactory m_ThrownObjectFactory;
    static CGameObjectFactory m_RecycledThrownObjectFactory;
    static CGameObjectFactory m_AttachObjectFactory;
    static CGameObjectFactory m_HierObjectFactory;
    static CGameObjectFactory m_TankObjectFactory;
    static CGameObjectFactory m_HierAnimObjectFactory;
    static void Shutdown();
};
extern StaticObjectDefinition *g_pStaticObjectTemplates;
extern int g_numTemplates;
int CompareStaticObjectDefinitions(const void *, const void *);
extern int g_iMsgVerbosityLevel;

void CStaticObjectFactory::Shutdown() {
    m_StaticObjectFactory.Shutdown();
    m_ThrownObjectFactory.Shutdown();
    m_RecycledThrownObjectFactory.Shutdown();
    m_AttachObjectFactory.Shutdown();
    m_HierObjectFactory.Shutdown();
    m_HierAnimObjectFactory.Shutdown();
    m_TankObjectFactory.Shutdown();
    delete[] g_pStaticObjectTemplates;
    g_pStaticObjectTemplates = 0;
    g_numTemplates = 0;
}
