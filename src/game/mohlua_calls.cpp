// A fragment of the original mohlua.cpp: state script calls that read a named value, start a death
// script and begin hurtling. Names come from the symbol table. The event numbers' meanings are not
// established here. BeginHurtling reports StartScriptDeath's message, as the original does.
#include "SceneObjects.h"
#include "ScriptRuntime.h"
#include "ScriptStartup.h"
#include "ScriptTimers.h"

struct lua_State;
extern "C" int lua_gettop(lua_State *);
extern "C" int lua_isstring(lua_State *, int);
extern "C" const char *lua_tostring(lua_State *, int);
extern "C" void lua_pushnumber(lua_State *, double);
extern "C" void lua_error(lua_State *, const char *);

CAnimObject *LUAGetCurrentObject();

// The object's second table, at +0x38; the symbol table names the slots.
class AnimObjectChoreographerClient {
  public:
    virtual void MoveTo();
    virtual void GetSkeleton();
    virtual void GetPosition();
    virtual void GetRotation();
    virtual void SetPosition();
    virtual void SetRotation();
    virtual void StartAnimCallback();
    virtual void UpdateAnimCallback();
    virtual float GetValue(const char *);
};

static int LUAGetStateVariable(lua_State *L) {
    if (lua_gettop(L) != 1)
        lua_error(L, "Incorrect argument to function 'GetStateVariable'");
    if (!lua_isstring(L, 1))
        lua_error(L, "Parameter passed to 'GetStateVariable' should be string");
    CAnimObject *object = LUAGetCurrentObject();
    const char *name = lua_tostring(L, 1);
    lua_pushnumber(L, ((AnimObjectChoreographerClient *)((char *)object + 0x38))->GetValue(name));
    return 1;
}

static int LUAStartScriptDeath(lua_State *L) {
    if (lua_gettop(L) != 1)
        lua_error(L, "Incorrect argument to function 'StartScriptDeath'");
    if (!lua_isstring(L, 1))
        lua_error(L, "Parameter passed to 'StartScriptDeath' should be string");
    ISceneNode *object = (ISceneNode *)LUAGetCurrentObject();
    BSObject *script;
    CreateBSObject(lua_tostring(L, 1), &script, 0, 0, 0);
    BSObjectTriggerEvent((BSObject *)object->GetScriptObject(), 336, 0, script, 1);
    BSObjectTriggerEvent(script, 41, 0, 0, 1);
    BSObjectTriggerEvent(script, 269, (void *)object->GetScriptObject(), (BSObject *)object->GetScriptObject(), 0);
    return 0;
}

static int LUABeginHurtling(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'StartScriptDeath'");
    LUAGetCurrentObject()->BeginHurtling();
    return 0;
}
