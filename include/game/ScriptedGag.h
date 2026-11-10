#ifndef SCRIPTEDGAG_H
#define SCRIPTEDGAG_H
// CScriptedGag: a table of script actions registered under a gag name's CRC, and the table of gags.
// Class and method names come from the symbol table; member names are inferred from the code.
#include "profile.h"
#include <map>

struct BSObject;

typedef _STL::map<int, BSObject *> ScriptActionMap;

class CScriptedGag {
  public:
    CScriptedGag();
    ~CScriptedGag();
    static void RegisterGagAction(BSObject *, const char *, int);
    static void UnregisterAllGagActions();
    static void StartGag(BSObject *, const char *, int);
    static void StartGag(BSObject *, unsigned int, int);
    void RegisterAction(BSObject *, int);
    void StartAction(BSObject *, int);

    ScriptActionMap actions;
};

typedef _STL::map<unsigned int, CScriptedGag *> ScriptedGagMap;

#endif
