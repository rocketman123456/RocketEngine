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

    //register the functions with lua
    lua_register(pL, "cpp_GetAIMove", cpp_GetAIMove);
    lua_register(pL, "cpp_EvaluateTheGuesses", cpp_EvaluateTheGuesses);

    //run the script
    std::string root = FindRootDir("_root_dir_");
    std::string script = root + "/Asset/Script/rock_paper_scissors2.lua";
    RunLuaScript(pL, script.c_str());

    //tidy up
    lua_close(pL);

    return 0;
}