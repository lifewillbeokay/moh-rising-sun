// FEng list and node classes; see docs/Frontend.md. Class tags and methods are from symbols;
// member names describe what the code does with them.
#ifndef GAME_FENG_LISTS_H
#define GAME_FENG_LISTS_H
#pragma interface

class FEMinNode {
  public:
    FEMinNode *next; // +0x0
    FEMinNode *prev; // +0x4
    FEMinNode() { prev = next = (FEMinNode *)0xabadcafe; }
    virtual ~FEMinNode() {} // table at +0x8
};

class FENode : public FEMinNode {
  public:
    char *name;         // +0xc
    unsigned long hash; // +0x10
    FENode();
    virtual ~FENode();
    void SetName(const char *);
};

class FEMinList {
  public:
    int count;       // +0x0
    FEMinNode *head; // +0x4
    FEMinNode *tail; // +0x8
    FEMinList() { head = 0; tail = 0; count = 0; }
    virtual ~FEMinList() { Purge(); } // table at +0xc
    void Purge();
    void AddTail(FEMinNode *node) { AddNode(tail, node); }
    void AddNode(FEMinNode *, FEMinNode *);
    FEMinNode *RemNode(FEMinNode *);
    FEMinNode *RemHead();
    FEMinNode *FindNode(unsigned long) const;
};

class FEList : public FEMinList {
  public:
    FENode *FindNode(const char *, FENode *) const;
    FENode *FindNode(const char *) const;
};

// A list that can instead refer to another list's nodes.
class FERefList {
  public:
    int isReference;  // +0x0
    FEMinNode *head;  // +0x4; the referenced FERefList when isReference is set.
    FEMinNode *tail;  // +0x8
    FERefList() { head = 0; tail = 0; isReference = 0; }
    virtual ~FERefList() { if (!isReference) Purge(); } // table at +0xc
    void Purge();
    void ReferenceList(FERefList *);
    void AddNode(FEMinNode *, FEMinNode *);
    FEMinNode *RemNode(FEMinNode *);
    FEMinNode *RemHead();
    int GetNumElements();
    FEMinNode *GetHead() const { return isReference ? ((FERefList *)head)->GetHead() : head; }
};

class FEWideString;
// Original enum tag; GetDataOffset and FETrackOffsets take the eleven values.
enum FEKeyTrack_Indices { FEKeyTrackIndex0 = 0 };
class FEScript;
class FEMessageResponse;
class FEObject;
// Called with an object that is being destroyed; slot one of its table.
class FEObjectDestructorCallback {
  public:
    virtual void ObjectDestroyed(FEObject *);
};

// Not an FENode: its name and hash are laid out differently.
class FEObject : public FEMinNode {
  public:
    static FEObjectDestructorCallback *pDestructorCallback;
    FEObject();
    FEObject(const FEObject &, bool);
    virtual ~FEObject();
    void SetName(const char *);
    FEObject *Clone(bool);
    long GetDataOffset(FEKeyTrack_Indices);
    FEScript *FindScript(unsigned long) const;
    void SetCurrentScript(FEScript *);
    void SetupMoveToTracks();
    void SetDataSize(unsigned long);
    FEMessageResponse *FindResponse(unsigned long) const;
    void SetScript(unsigned long, bool);
    void SetScript(FEScript *, bool);
    unsigned long guid;           // +0x0c
    unsigned long hash;           // +0x10
    char *name;                   // +0x14
    int type;                     // +0x18; 5 for a group, 2 for a string in SetGroupString.
    union {
        unsigned int flags;           // +0x1c, the object's flag word.
        struct {
            unsigned int unknownBits : 5;
            unsigned int disabled : 1;    // Set by DisableButton.
            unsigned int unknownBit6 : 3;
            unsigned int changed : 1;     // Set whenever text or script is replaced.
        };
    };
    unsigned short unknown_20;    // +0x20
    unsigned short resourceIndex; // +0x22; 0xffff when the object uses no resource.
    void *resourceData;           // +0x24
    void *resourceHandle;         // +0x28
    unsigned char *data;          // +0x2c
    unsigned long dataSize;       // +0x30
    FEMinList responseList;       // +0x34
    FEMinList scriptList;         // +0x44
    FEScript *currentScript;      // +0x54
};

class FEGroup : public FEObject {
  public:
    FEObject *FindChild(unsigned long) const;
    FEObject *FindChild(const char *) const;
    unsigned char unknown_58[0x5c - 0x58];
    FEObject *children; // +0x5c
};

// A growable array of the objects that receive one message.
class FEMsgTargetList {
  public:
    unsigned long message;   // +0x0
    unsigned long allocated; // +0x4
    unsigned long count;     // +0x8
    FEObject **targets;      // +0xc
    void Allocate(unsigned long);
    void AppendTarget(FEObject *);
};

// Called for each object by FEPackage::ForAllObjects; a zero result stops the walk.
class FEObjectCallback {
  public:
    FEObjectCallback() {}
    virtual ~FEObjectCallback() {}
    virtual int Callback(FEObject *) = 0;
};

class FEFindByHash : public FEObjectCallback {
  public:
    unsigned long hash; // +0x4
    FEObject *found;    // +0x8
    FEFindByHash() {}
    virtual int Callback(FEObject *);
};

class FEFindByGUID : public FEObjectCallback {
  public:
    unsigned long guid; // +0x4
    FEObject *found;    // +0x8
    FEFindByGUID() {}
    virtual int Callback(FEObject *);
};

class PackageInitStateCB : public FEObjectCallback {
  public:
    virtual int Callback(FEObject *);
};

class FEPackage;
// 24 bytes each, per ResourceConnector.
struct FEResourceRequest {
    unsigned char unknown_0[0x10];
    void *handle; // +0x10; stored at +0x24 of the object.
    void *data;   // +0x14; stored at +0x28 of the object.
    unsigned char unknown_18[0x18 - 0x18];
};
class FEListBox;
class ResourceConnector : public FEObjectCallback {
  public:
    FEPackage *package;           // +0x4
    FEResourceRequest **requests; // +0x8
    ResourceConnector() {}
    virtual int Callback(FEObject *);
    void ConnectListBoxResources(FEListBox *);
};

class FEGameInterface;
class FEngine;
struct FELibraryReference {
    unsigned long id;  // +0x0
    long unknown_4;    // +0x4
    long unknown_8;    // +0x8
    FELibraryReference() : id(0), unknown_4(-1), unknown_8(0) {}
};
class FEPackage : public FENode {
  public:
    FEMsgTargetList *GetMessageTargets(unsigned long);
    FEObject *FindObjectByName(const char *);
    FEObject *FindObjectByHash(unsigned long);
    FEObject *FindObjectByGUID(unsigned long);
    void SetFilename(const char *);
    void UpdateObject(FEObject *, long);
    void UpdateGroup(FEGroup *, long);
    void Update(FEngine *, long);
    void SetCurrentButton(FEngine *, FEObject *, bool);
    int Startup(FEGameInterface *);
    void Shutdown(FEGameInterface *);
    FEMessageResponse *FindResponse(unsigned long);
    int ForAllChildren(FEGroup *, FEObjectCallback &);
    int ForAllObjects(FEObjectCallback &);
    void ConnectObjectResources();
    void SetNumLibraryRefs(unsigned long);
    const FELibraryReference *FindLibraryReference(unsigned long) const;
    int executing;                    // +0x14
    unsigned char unknown_18[0x24 - 0x18];
    int priority;                     // +0x24
    int controlMask;                  // +0x28
    unsigned char unknown_2c[0x30 - 0x2c];
    FEngine *engine;                  // +0x30; set by Update.
    long lastDeltaTime;               // +0x34; set by Update.
    char *fileName;                   // +0x38
    const FEPackage *parent;          // +0x3c
    unsigned char unknown_40[0x48 - 0x40];
    FEObject *firstObject;            // +0x48
    unsigned char unknown_4c[0x58 - 0x4c];
    FEMessageResponse *firstResponse; // +0x58
    unsigned char unknown_5c[0x64 - 0x5c];
    unsigned long resourceCount;      // +0x64
    struct FEResourceRequest *resourceRequests; // +0x68
    unsigned long messageTargetCount; // +0x6c
    FEMsgTargetList *messageTargets;  // +0x70
    unsigned char unknown_74[0x84 - 0x74];
    unsigned long libraryRefCount;    // +0x84
    FELibraryReference *libraryRefs;  // +0x88
    FEObject *currentButton;          // +0x8c
    unsigned char unknown_90[0xac - 0x90];
    signed char groupId;              // +0xac
};

// Packages sorted by priority.
class FEPackageList : public FEMinList {
  public:
    void AddPackage(FEPackage *);
    FEPackage *FindPackage(const char *, unsigned char) const;
    FEPackage *FindPackage(unsigned char) const;
    int RemovePackage(FEPackage *);
    void ReplaceParentLinks(const FEPackage *, const FEPackage *);
};

unsigned int FEHashUpper(const char *);
int FEStricmp(const char *, const char *);

#endif
