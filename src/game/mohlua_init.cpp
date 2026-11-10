// A fragment of the original mohlua.cpp: the Lua state's lifetime and the state script entry point.
// Names come from the symbol table. The registered functions are the file's private statics, reached
// here as externals.
struct lua_State;
typedef int (*lua_CFunction)(lua_State *);
extern "C" lua_State *lua_open(int);
extern "C" void lua_close(lua_State *);
extern "C" void lua_pushcclosure(lua_State *, lua_CFunction, int);
extern "C" void lua_setglobal(lua_State *, const char *);
extern "C" void lua_mathlibopen(lua_State *);
extern "C" int lua_dobuffer(lua_State *, const char *, unsigned int, const char *);
extern "C" void lua_setgcthreshold(lua_State *, int);
void DebugMsg(const char *, ...);

int SetStateChoice(lua_State *);
int LUAGetWeaponType(lua_State *);
int LUAGetPosture(lua_State *);
int LUAGetBulletType(lua_State *);
int LUAGetHitLocation(lua_State *);
int LUAGetHitIntensity(lua_State *);
int LUAGetStateVariable(lua_State *);
int LUAStartScriptDeath(lua_State *);
int LUABeginHurtling(lua_State *);

extern int g_choice;
class CAnimObject;
extern CAnimObject *g_lua_object;
extern lua_State *g_state;

lua_State *LUAGetState();

void LUAInit() {
    g_state = lua_open(0);
    lua_pushcclosure(g_state, SetStateChoice, 0);
    lua_setglobal(g_state, "SetStateChoice");
    lua_mathlibopen(g_state);
    lua_pushcclosure(LUAGetState(), LUAGetWeaponType, 0);
    lua_setglobal(LUAGetState(), "GetWeaponType");
    lua_pushcclosure(LUAGetState(), LUAGetPosture, 0);
    lua_setglobal(LUAGetState(), "GetPosture");
    lua_pushcclosure(LUAGetState(), LUAGetBulletType, 0);
    lua_setglobal(LUAGetState(), "GetBulletType");
    lua_pushcclosure(LUAGetState(), LUAGetHitLocation, 0);
    lua_setglobal(LUAGetState(), "GetHitLocation");
    lua_pushcclosure(LUAGetState(), LUAGetHitIntensity, 0);
    lua_setglobal(LUAGetState(), "GetHitIntensity");
    lua_pushcclosure(LUAGetState(), LUAGetStateVariable, 0);
    lua_setglobal(LUAGetState(), "GetStateVariable");
    lua_pushcclosure(LUAGetState(), LUAStartScriptDeath, 0);
    lua_setglobal(LUAGetState(), "StartScriptDeath");
    lua_pushcclosure(LUAGetState(), LUABeginHurtling, 0);
    lua_setglobal(LUAGetState(), "BeginHurtling");
}

void LUAShutdown() {
    lua_close(g_state);
    g_state = 0;
}

int LUAEvaluateState(void *object, const char *buffer, int size, const char *name) {
    g_lua_object = (CAnimObject *)object;
    g_choice = 0;
    if (lua_dobuffer(g_state, buffer, size, name))
        DebugMsg("\n!!! The previous error was reported by the LUA subsystem !!!\n\n");
    lua_setgcthreshold(g_state, 0);
    return g_choice;
}

lua_State *LUAGetState() { return g_state; }

CAnimObject *LUAGetCurrentObject() { return g_lua_object; }
