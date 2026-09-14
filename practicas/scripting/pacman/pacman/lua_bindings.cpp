#include "lua_bindings.h"
#include "lua.hpp"

// Declaraciones extern de la librería del motor.
#include "engine_api.h"

// ------------------- Práctica 1: funciones sueltas -------------------

static int lua_setPacmanSpeedMultiplier(lua_State* L) {
    float s = (float)luaL_checknumber(L, 1);
    setPacmanSpeedMultiplier(s);
    return 0;
}

static int lua_setPacmanColor(lua_State* L) {
    int r = (int)luaL_checkinteger(L, 1);
    int g = (int)luaL_checkinteger(L, 2);
    int b = (int)luaL_checkinteger(L, 3);
    int a = (int)luaL_optinteger(L, 4, 255);
    setPacmanColor((unsigned char)r, (unsigned char)g,
        (unsigned char)b, (unsigned char)a);
    return 0;
}

static int lua_setPowerUpTime(lua_State* L) {
    int t = (int)luaL_checkinteger(L, 1);
    setPowerUpTime(t);
    return 0;
}

// ------------------- Práctica 2: registro de la clase Pacman -------------------

// Viene de Pacman_Lua.cpp
void registerPacmanClass(lua_State* L);

// ------------------- Registro global -------------------

void registerBindings(lua_State* L) {
    // Práctica 1
    lua_register(L, "setPacmanSpeedMultiplier", lua_setPacmanSpeedMultiplier);
    lua_register(L, "setPacmanColor", lua_setPacmanColor);
    lua_register(L, "setPowerUpTime", lua_setPowerUpTime);

    // Práctica 2
    registerPacmanClass(L);
}