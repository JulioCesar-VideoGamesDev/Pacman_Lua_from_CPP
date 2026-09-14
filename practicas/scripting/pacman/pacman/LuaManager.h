#pragma once
#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <string>
#include <ctime>

#include "lua.hpp"

class LuaManager {
public:
    static LuaManager& instance() {
        static LuaManager inst;
        return inst;
    }

private:
    LuaManager();
    ~LuaManager();
    LuaManager(const LuaManager&) = delete;
    LuaManager& operator=(const LuaManager&) = delete;

public:
    // Creates the lua_State and register the bindings.
    bool init();

    // Loads config.lua
    bool loadConfig(const std::string& filename);

    // If config.lua changed then loads it again.
    bool reloadIfNeeded();

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