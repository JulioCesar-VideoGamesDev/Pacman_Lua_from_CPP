#pragma once
#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

struct lua_State;

// Registra TODAS las funciones y clases expuestas a Lua.
void registerBindings(lua_State* L);

#endif