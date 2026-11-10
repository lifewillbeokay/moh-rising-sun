// A fragment of the original mohlua.cpp: queries the state scripts make on the current object. Names
// come from the symbol table; the script data's member names are inferred from what they return.
#include "SceneObjects.h"
#include "ScriptRuntime.h"
#include "BSGameObject.h"

struct lua_State;
extern "C" int lua_gettop(lua_State *);
extern "C" void lua_pushnumber(lua_State *, double);
extern "C" void lua_error(lua_State *, const char *);

CAnimObject *LUAGetCurrentObject();

struct LuaObjectScriptData {
    unsigned char unknown00[140];
    int hitIntensity; // +0x8c
    unsigned char unknown90[4];
    int bulletType;   // +0x94
    int hitLocation;  // +0x98
    int posture;      // +0x9c
};

static inline LuaObjectScriptData *CurrentScriptData() {
    BSObject *script = (BSObject *)((ISceneNode *)LUAGetCurrentObject())->GetScriptObject();
    return (LuaObjectScriptData *)script->gameObject->GetScriptData();
}

static int LUAGetWeaponType(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'GetWeaponType'");
    int type;
    switch (((CSoldierObject *)LUAGetCurrentObject())->GetCurrentWeaponType()) {
    default:
        type = 0;
        break;
    case 1:
        type = 1;
        break;
    case 2:
    case 3:
        type = 2;
        break;
    case 5:
        type = 3;
        break;
    case 6:
        type = 4;
        break;
    }
    lua_pushnumber(L, type);
    return 1;
}

static int LUAGetPosture(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'GetPosture'");
    lua_pushnumber(L, CurrentScriptData()->posture);
    return 1;
}

static int LUAGetBulletType(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'GetBulletType'");
    int type;
    switch (CurrentScriptData()->bulletType) {
    case 2:
    case 6:
    case 18:
    case 19:
    case 20:
        type = 1;
        break;
    case 3:
    case 24:
        type = 2;
        break;
    default:
        type = 0;
        break;
    }
    lua_pushnumber(L, type);
    return 1;
}

static int LUAGetHitLocation(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'GetHitLocation'");
    lua_pushnumber(L, CurrentScriptData()->hitLocation);
    return 1;
}

static int LUAGetHitIntensity(lua_State *L) {
    if (lua_gettop(L) != 0)
        lua_error(L, "Incorrect argument to function 'GetHitIntensity'");
    lua_pushnumber(L, CurrentScriptData()->hitIntensity);
    return 1;
}
