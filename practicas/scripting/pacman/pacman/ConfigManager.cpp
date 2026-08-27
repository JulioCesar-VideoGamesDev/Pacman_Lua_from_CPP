#include "ConfigManager.h"

#include <iostream>
#include <lua.hpp>

ConfigManager::ConfigManager() {
    setDefaultValues();
}

ConfigManager::~ConfigManager() {}

void ConfigManager::setDefaultValues() {
    powerUpScore = 5000;
    powerUpDuration = 5.0f;
    powerUpSpeedMultiplier = 2.0f;
    pacmanPowerUpColor = { 0, 255, 0 };
    bronzeMedalPoints = 100;
    coinPoints = 50;
}

bool ConfigManager::loadConfig(const std::string& filename) {
    // Create the LuaState
    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Could not create the LuaState" << std::endl;
        return false;
    }

    // Load the standar library
    luaL_openlibs(L);

    // Load the config file
    if (luaL_loadfile(L, filename.c_str()) != 0) {
        std::cerr << "Error to load the file" << filename << ": " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        return false;
    }

    // Execute the file
    if (lua_pcall(L, 0, 0, 0) != 0) {
        std::cerr << "Error to execute file" << filename << ": " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        return false;
    }

    // powerUpScore
    lua_getglobal(L, "powerUpScore");
    if (lua_isnumber(L, -1)) {
        powerUpScore = lua_tointeger(L, -1);
        std::cout << "powerUpScore loaded: " << powerUpScore << std::endl;
    }
    else {
        std::cerr << "powerUpScore not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // powerUpDuration
    lua_getglobal(L, "powerUpDuration");
    if (lua_isnumber(L, -1)) {
        powerUpDuration = lua_tonumber(L, -1);
        std::cout << "powerUpDuration loaded: " << powerUpDuration << std::endl;
    }
    else {
        std::cerr << "powerUpDuration not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // powerUpSpeedMultiplier
    lua_getglobal(L, "powerUpSpeedMultiplier");
    if (lua_isnumber(L, -1)) {
        powerUpSpeedMultiplier = lua_tonumber(L, -1);
        std::cout << "powerUpSpeedMultiplier loaded: " << powerUpSpeedMultiplier << std::endl;
    }
    else {
        std::cerr << "powerUpSpeedMultiplier not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // pacmanPowerUpColor (tabla)
    lua_getglobal(L, "pacmanPowerUpColor");
    if (lua_istable(L, -1)) {
        // Read r
        lua_getfield(L, -1, "r");
        if (lua_isnumber(L, -1)) {
            pacmanPowerUpColor.r = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        // Read g
        lua_getfield(L, -1, "g");
        if (lua_isnumber(L, -1)) {
            pacmanPowerUpColor.g = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        // Read b
        lua_getfield(L, -1, "b");
        if (lua_isnumber(L, -1)) {
            pacmanPowerUpColor.b = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        std::cout << "pacmanPowerUpColor loaded: (" << pacmanPowerUpColor.r << ","
            << pacmanPowerUpColor.g << "," << pacmanPowerUpColor.b << ")" << std::endl;
    }
    else {
        std::cerr << "pacmanPowerUpColor not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // bronzeMedalPoints
    lua_getglobal(L, "bronzeMedalPoints");
    if (lua_isnumber(L, -1)) {
        bronzeMedalPoints = lua_tointeger(L, -1);
        std::cout << "bronzeMedalPoints loaded: " << bronzeMedalPoints << std::endl;
    }
    else {
        std::cerr << "bronzeMedalPoints not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // coinPoints
    lua_getglobal(L, "coinPoints");
    if (lua_isnumber(L, -1)) {
        coinPoints = lua_tointeger(L, -1);
        std::cout << "coinPoints loaded: " << coinPoints << std::endl;
    }
    else {
        std::cerr << "coinPoints not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    lua_close(L);
    return true;
}