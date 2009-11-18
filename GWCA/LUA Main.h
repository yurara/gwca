#ifndef _LUA_MAIN_H
#define _LUA_MAIN_H

#include "LUA\include\lua.hpp"
#include "stdafx.h"
#include "Interface.h"

extern lua_State* g_LuaInstance;

void init_lua();
void run_script(char* script);
void close_lua();

#endif