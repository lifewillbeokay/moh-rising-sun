// CFengInterface: an FEng game interface drawing with EAGL. Scoped view; see docs/Frontend.md.
#ifndef GAME_FENG_INTERFACE_H
#define GAME_FENG_INTERFACE_H
#pragma interface
#include "profile.h"
#include "FEngMath.h"
#include "FEngObjects.h"
#include "PauseMenu.h"
#include <map>
// A set of UI objects. The tree and its helpers are symbols; the project has no <set>, so this is
// the part of the STLport class these methods use.
class CUIObjectSet {
  public:
    typedef _STL::_Rb_tree<CUIObject *, CUIObject *, _STL::_Identity<CUIObject *>, _STL::less<CUIObject *>,
                           _STL::allocator<CUIObject *> > Tree;
    typedef Tree::iterator iterator;
    iterator begin() { return tree.begin(); }
    iterator end() { return tree.end(); }
    _STL::pair<iterator, bool> insert(CUIObject *const &object) {
        _STL::pair<iterator, bool> result = tree.insert_unique(object);
        return result;
    }
    Tree tree;
};
extern "C" void *DWI_alloc(const char *, int, int);
struct ShapeFile;
class CSprite {
  public:
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
    enum ESpriteLoadType { SpriteLoadUnknown0 = 0 };
    CSprite(const char *, ESpriteLoadType);
    CSprite(ShapeFile *);
    ~CSprite();
    unsigned char unknown_0[36];
};
class CFont {
  public:
    void *operator new(unsigned int size, const char *label) { return DWI_alloc(label, size, 1024); }
    CFont(char *);
    ~CFont();
    unsigned char unknown_0[20];
};
class FEImage;
class FESimpleImage;
class FEString;
class CFengInterface : public FEGameInterface {
  public:
    CFengInterface(FEngine *);
    virtual ~CFengInterface();
    void SetBigFilePtr(char *);
    int GetPackageData(const char *, unsigned char **, bool &);
    int LocalizePackage(FEPackage *);
    int LoadResources(FEPackage *, long, FEResourceRequest *);
    int UnloadResources(FEPackage *, long, FEResourceRequest *);
    void PackageWasLoaded(FEPackage *);
    int PackageWillUnload(FEPackage *);
    void BeginPackageRendering(FEPackage *);
    void EndPackageRendering(FEPackage *);
    void GenerateRenderContext(unsigned short, FEObject *);
    int GetContextTransform(unsigned short, FEMatrix4 &);
    void GetViewTransformation(FEMatrix4 *);
    void RenderObject(FEObject *);
    unsigned long GetJoyPadMask(unsigned char);
    void ReplaceFilenameExtension(char *, char *);
    int LoadTexture(FEResourceRequest *, char *);
    int LoadFont(FEResourceRequest *, char *);
    void GenerateXformMatrix(EAGL::Transform *, FEObjData *, unsigned short);
    void RenderImage(FEImage *);
    void Render(FESimpleImage *);
    void RenderString(FEString *);
    void NotificationMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    void NotifySoundMessage(unsigned long, FEObject *, unsigned long, unsigned long);
    void RegisterUIObject(CUIObject *);
    void DebugMessageQueued(unsigned long, FEObject *, FEPackage *, FEObject *, unsigned long);
    void DebugMessageProcessed(unsigned long, FEObject *, FEObject *, FEPackage *, unsigned long);
    void GetMouseInfo(FEMouseInfo &);
    unsigned char unknown_4[0x60 - 4];
    SFengRenderContext contexts[64]; // +0x60
    char *bigFile;                   // +0x1860
    FEngine *engine;                 // +0x1864
    int lastStickX;                  // +0x1868; the stick's last value in GetJoyPadMask.
    int lastStickY;                  // +0x186c
    int unknown_1870;                // +0x1870; cleared by UnloadResources.
    CUIObjectSet uiObjects; // +0x1874; objects told about each message.
    unsigned char unknown_1884[0x18a0 - 0x1884]; // Allocation extent in CSaveGamePopup.
};
#endif
