// A fragment of the original mohlua.cpp: the state script's choice call. Names come from the symbol
// table; g_choice is the file's private static, reached here as an external.
struct lua_State;
extern "C" int lua_gettop(lua_State *);
extern "C" int lua_isnumber(lua_State *, int);
extern "C" double lua_tonumber(lua_State *, int);
extern "C" void lua_error(lua_State *, const char *);

extern int g_choice;

static int SetStateChoice(lua_State *L) {
    if (lua_gettop(L) != 1 || !lua_isnumber(L, 1))
        lua_error(L, "Incorrect argument to function 'SetStateChoice'");
    g_choice = (int)lua_tonumber(L, 1);
    return 0;
}
