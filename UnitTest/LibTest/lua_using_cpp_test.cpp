extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include "Utils/FindRootDir.h"

#include <iostream>
#include <string>

using namespace std;
using namespace Rocket;

#include "lua_helper_function.h"
#include "rock_paper_scissor.h"

int main()
{
    //create a lua state
    lua_State *pL = luaL_newstate();

    OpenLuaLibraries(pL);
    // luaopen_base(pL);
    // luaopen_string(pL);
    // luaopen_table(pL);
    // luaopen_math(pL);
    // luaopen_io(pL);
    // luaopen_os(pL);
    // luaopen_debug(pL);
    // luaopen_package(pL);
    // luaopen_utf8(pL);

    // luaL_requiref(pL, LUA_GNAME, luaopen_base, 1);
    // lua_pop(pL, 1);
    // luaL_requiref(pL, LUA_STRLIBNAME, luaopen_string, 1);
    // lua_pop(pL, 1);
    // luaL_requiref(pL, LUA_MATHLIBNAME, luaopen_math, 1);
    // lua_pop(pL, 1);
    // luaL_requiref(pL, LUA_OSLIBNAME, luaopen_os, 1);
    // lua_pop(pL, 1);

    // lua_setglobal(pL,"string");
    // lua_setglobal(pL,"math");

    //register the functions with lua
    // lua_pushcfunction(pL, cpp_GetAIMove);
    // lua_setglobal(pL, "cpp_GetAIMove");
    // lua_pushcfunction(pL, cpp_GetAIMove);
    // lua_setglobal(pL, "cpp_EvaluateTheGuesses");
    lua_register(pL, "cpp_GetAIMove", cpp_GetAIMove);
    lua_register(pL, "cpp_EvaluateTheGuesses", cpp_EvaluateTheGuesses);

    //run the script
    std::string root = FindRootDir("_root_dir_");
    std::string script = root + "/Asset/Script/rock_paper_scissors2.lua";
    // RunLuaScript(pL, script.c_str());
    if (int error = luaL_dofile(pL, script.c_str()) != 0) {
        std::cout << ttos(error) << std::endl;
    }
    //tidy up
    lua_close(pL);

    return 0;
}