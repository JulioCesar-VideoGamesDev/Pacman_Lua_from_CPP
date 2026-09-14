#include "LuaManager.h"

#include <iostream>
#include <sys/stat.h>

// Aquí están las declaraciones de los bindings
#include "lua_bindings.h"

LuaManager::LuaManager() : L(nullptr), lastFileModTime(0) {}

LuaManager::~LuaManager() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

bool LuaManager::init()
{
    if (L) return true;   // ya inicializado

    L = luaL_newstate();
    if (!L) {
        std::cerr << "Could not create the LuaState" << std::endl;
        return false;
    }
    luaL_openlibs(L);
    registerBindings(L);
    return true;
}

time_t LuaManager::getFileModTime(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) == 0) {
        return fileInfo.st_mtime;
    }
    return 0;
}

bool LuaManager::checkIfFileChanged() {
    if (configFilename.empty()) return false;
    time_t currentModTime = getFileModTime(configFilename);
    if (currentModTime != lastFileModTime) {
        lastFileModTime = currentModTime;
        std::cout << "config.lua has changed" << std::endl;
        return true;
    }
    return false;
}

bool LuaManager::loadConfigInternal(const std::string& filename) {
    // En lugar de crear el L aquí, asegúrate de que existe
    if (!L) {
        if (!init()) return false;
    }
    else {
        // Ya existe: limpiamos las globals y bindings para recargar limpio.
        // La forma más simple es cerrar y volver a crear. Pero eso
        // re-registraría los bindings, que es justo lo que queremos.
        lua_close(L);
        L = nullptr;
        if (!init()) return false;
    }

    if (luaL_loadfile(L, filename.c_str()) != 0) {
        std::cerr << "Error loading file " << filename << ": "
            << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }

    if (lua_pcall(L, 0, 0, 0) != 0) {
        std::cerr << "Error executing file " << filename << ": "
            << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }

    return true;
}

bool LuaManager::loadConfig(const std::string& filename) {
    configFilename = filename;
    lastFileModTime = getFileModTime(filename);
    std::cout << "Loading configuration from: " << filename << std::endl;
    return loadConfigInternal(filename);
}

bool LuaManager::reloadIfNeeded() {
    if (!checkIfFileChanged()) return false;
    std::cout << "Reloading configuration..." << std::endl;
    return loadConfigInternal(configFilename);
}