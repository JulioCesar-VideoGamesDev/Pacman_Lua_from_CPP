#pragma once
#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

struct lua_State;

// Register ALL the functions and the exposed class to Lua.
void registerBindings(lua_State* L);

#endif