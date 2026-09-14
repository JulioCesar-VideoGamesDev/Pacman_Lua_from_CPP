#pragma once
#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <string>
#include <ctime>

#include "lua.hpp"

class LuaManager {
public:
    static LuaManager& instance() {
        static LuaManager inst;   // se construye la primera vez que se llama
        return inst;
    }

private:
    LuaManager();   // privado para forzar uso de instance()
    ~LuaManager();
    LuaManager(const LuaManager&) = delete;
    LuaManager& operator=(const LuaManager&) = delete;

public:
    // Crea el lua_State y registra bindings. NO ejecuta ningún script.
    bool init();

    // Carga config.lua y registra los bindings (funciones + clase Pacman).
    bool loadConfig(const std::string& filename);

    // Recarga si el fichero ha cambiado. Devuelve true si se recargó.
    bool reloadIfNeeded();

    // Acceso al estado de Lua (para que main.cpp pueda leer globales como "pacman")
    lua_State* getLuaState() const { return L; }

private:
    lua_State* L;
    std::string configFilename;
    time_t lastFileModTime;

    time_t getFileModTime(const std::string& filename);
    bool checkIfFileChanged();
    bool loadConfigInternal(const std::string& filename);
};

#endif