#include "lua.hpp"
#include "Pacman.h"

#include <new>

// Constructor / _gc ----------------

static int pacman_new(lua_State* L) {
    void* ud = lua_newuserdata(L, sizeof(Pacman));
    new (ud) Pacman();                      // placement new
    luaL_getmetatable(L, "Pacman");
    lua_setmetatable(L, -2);
    return 1;
}

static int pacman_gc(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    p->~Pacman();
    return 0;
}

// Methods ----------------

static int pacman_setSpeedMultiplier(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    float s = (float)luaL_checknumber(L, 2);
    p->setSpeedMultiplier(s);
    return 0;
}

static int pacman_setColor(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    int r = (int)luaL_checkinteger(L, 2);
    int g = (int)luaL_checkinteger(L, 3);
    int b = (int)luaL_checkinteger(L, 4);
    int a = (int)luaL_optinteger(L, 5, 255);
    p->setColor(r, g, b, a);
    return 0;
}

static int pacman_setPowerUpTime(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    int t = (int)luaL_checkinteger(L, 2);
    p->setPowerUpTime(t);
    return 0;
}

static int pacman_setMaxLives(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    float l = (float)luaL_checknumber(L, 2);
    p->setMaxLives(l);
    return 0;
}

static int pacman_getLives(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    lua_pushnumber(L, p->getLives());
    return 1;
}

static int pacman_getCoins(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    lua_pushinteger(L, p->getCoins());
    return 1;
}

static int pacman_reset(lua_State* L) {
    Pacman* p = (Pacman*)luaL_checkudata(L, 1, "Pacman");
    p->reset();
    return 0;
}

// Methods table for Lua ----------------

static const luaL_Reg pacman_methods[] = {
    { "setSpeedMultiplier", pacman_setSpeedMultiplier },
    { "setColor",           pacman_setColor },
    { "setPowerUpTime",     pacman_setPowerUpTime },
    { "setMaxLives",        pacman_setMaxLives },
    { "getLives",           pacman_getLives },
    { "getCoins",           pacman_getCoins },
    { "reset",              pacman_reset },
    { "__gc",               pacman_gc },
    { nullptr, nullptr }
};

void registerPacmanClass(lua_State* L) {
    // Creates the metatable
    luaL_newmetatable(L, "Pacman");
    
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // Register all the methods
    for (const luaL_Reg* r = pacman_methods; r->name; ++r) {
        lua_pushcfunction(L, r->func);
        lua_setfield(L, -2, r->name);
    }

    lua_pop(L, 1);

    // Global constructor
    lua_register(L, "Pacman", pacman_new);
}