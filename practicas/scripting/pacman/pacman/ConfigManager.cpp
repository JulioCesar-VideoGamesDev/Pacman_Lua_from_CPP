#include "ConfigManager.h"

#include <iostream>

ConfigManager::ConfigManager() : L(nullptr), lastFileModTime(0), needsReload(true) {
    setDefaultValues();
}

ConfigManager::~ConfigManager()
{
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }
}

void ConfigManager::setDefaultValues()
{
    powerUpScore = 5000;
    powerUpDuration = 5.0f;
    powerUpSpeedMultiplier = 2.0f;
    pacmanPowerUpColor = { 0, 255, 0 };
    bronzeMedalPoints = 100;
    coinPoints = 50;
}

// To get the TimeStamp of the file.
time_t ConfigManager::getFileModTime(const std::string& filename)
{
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) == 0)
    {
        return fileInfo.st_mtime;
    }
    return 0;
}

bool ConfigManager::checkIfFileChanged() 
{
    if (configFilename.empty())
    {
        return false;
    }

    time_t currentModTime = getFileModTime(configFilename);
    if (currentModTime != lastFileModTime)
    {
        lastFileModTime = currentModTime;
        std::cout << "config.lua has changed" << std::endl;
        return true;
    }
    return false;
}

bool ConfigManager::loadConfigInternal(const std::string& filename)
{
    // If we already have a LuaState, then we close it.
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }

    // Create the LuaState
    L = luaL_newstate();
    if (!L)
    {
        std::cerr << "Could not create the LuaState" << std::endl;
        return false;
    }

    // Load the standar library
    luaL_openlibs(L);

    // Load the config file
    if (luaL_loadfile(L, filename.c_str()) != 0)
    {
        std::cerr << "Error to load the file" << filename << ": " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        L = nullptr;
        return false;
    }

    // Execute the file
    if (lua_pcall(L, 0, 0, 0) != 0)
    {
        std::cerr << "Error to execute file" << filename << ": " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        L = nullptr;
        return false;
    }

    // powerUpScore
    lua_getglobal(L, "powerUpScore");
    if (lua_isnumber(L, -1))
    {
        powerUpScore = lua_tointeger(L, -1);
        std::cout << "powerUpScore loaded: " << powerUpScore << std::endl;
    }
    else
    {
        std::cerr << "powerUpScore not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // powerUpDuration
    lua_getglobal(L, "powerUpDuration");
    if (lua_isnumber(L, -1))
    {
        powerUpDuration = lua_tonumber(L, -1);
        std::cout << "powerUpDuration loaded: " << powerUpDuration << std::endl;
    }
    else
    {
        std::cerr << "powerUpDuration not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // powerUpSpeedMultiplier
    lua_getglobal(L, "powerUpSpeedMultiplier");
    if (lua_isnumber(L, -1))
    {
        powerUpSpeedMultiplier = lua_tonumber(L, -1);
        std::cout << "powerUpSpeedMultiplier loaded: " << powerUpSpeedMultiplier << std::endl;
    }
    else
    {
        std::cerr << "powerUpSpeedMultiplier not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // pacmanPowerUpColor (tabla)
    lua_getglobal(L, "pacmanPowerUpColor");
    if (lua_istable(L, -1)) {
        // Read r
        lua_getfield(L, -1, "r");
        if (lua_isnumber(L, -1))
        {
            pacmanPowerUpColor.r = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        // Read g
        lua_getfield(L, -1, "g");
        if (lua_isnumber(L, -1))
        {
            pacmanPowerUpColor.g = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        // Read b
        lua_getfield(L, -1, "b");
        if (lua_isnumber(L, -1))
        {
            pacmanPowerUpColor.b = lua_tointeger(L, -1);
        }
        lua_pop(L, 1);

        std::cout << "pacmanPowerUpColor loaded: (" << pacmanPowerUpColor.r << ","
            << pacmanPowerUpColor.g << "," << pacmanPowerUpColor.b << ")" << std::endl;
    }
    else
    {
        std::cerr << "pacmanPowerUpColor not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // bronzeMedalPoints
    lua_getglobal(L, "bronzeMedalPoints");
    if (lua_isnumber(L, -1))
    {
        bronzeMedalPoints = lua_tointeger(L, -1);
        std::cout << "bronzeMedalPoints loaded: " << bronzeMedalPoints << std::endl;
    }
    else
    {
        std::cerr << "bronzeMedalPoints not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    // coinPoints
    lua_getglobal(L, "coinPoints");
    if (lua_isnumber(L, -1))
    {
        coinPoints = lua_tointeger(L, -1);
        std::cout << "coinPoints loaded: " << coinPoints << std::endl;
    }
    else
    {
        std::cerr << "coinPoints not found, using default values" << std::endl;
    }
    lua_pop(L, 1);

    return true;

    // Verify that the function getPowerUpColor exists.
    lua_getglobal(L, "getPowerUpColor");
    if (!lua_isfunction(L, -1))
    {
        std::cerr << "Function getPowerUpColor not found in Lua" << std::endl;
    }
    lua_pop(L, 1);

    return true;
}

bool ConfigManager::loadConfig(const std::string& filename)
{
    // Save the name of the file for the reload.
    configFilename = filename;
    lastFileModTime = getFileModTime(filename);

    std::cout << "Loading configuration from: " << filename << std::endl;

    bool result = loadConfigInternal(filename);

    return result;
}

bool ConfigManager::reloadIfNeeded()
{
    if (!checkIfFileChanged())
    {
        return false;
    }

    std::cout << "Reloading configuration..." << std::endl;

    // Save current values in case the reload fails.
    int oldScore = powerUpScore;
    float oldDuration = powerUpDuration;
    float oldSpeed = powerUpSpeedMultiplier;
    Color oldColor = pacmanPowerUpColor;
    int oldBronze = bronzeMedalPoints;
    int oldCoins = coinPoints;

    // Try to reload
    if (!loadConfigInternal(configFilename)) 
    {
        // Reset values since the reload failed.
        powerUpScore = oldScore;
        powerUpDuration = oldDuration;
        powerUpSpeedMultiplier = oldSpeed;
        pacmanPowerUpColor = oldColor;
        bronzeMedalPoints = oldBronze;
        coinPoints = oldCoins;
        return false;
    }

    return true;
}

int ConfigManager::getPowerUpScore()
{
    reloadIfNeeded();
    return powerUpScore;
}

float ConfigManager::getPowerUpDuration()
{
    reloadIfNeeded();
    return powerUpDuration;
}

float ConfigManager::getPowerUpSpeedMultiplier()
{
    reloadIfNeeded();
    return powerUpSpeedMultiplier;
}

ConfigManager::Color ConfigManager::getPacmanPowerUpColor()
{
    reloadIfNeeded();
    return pacmanPowerUpColor;
}

int ConfigManager::getBronzeMedalPoints()
{
    reloadIfNeeded();
    return bronzeMedalPoints;
}

int ConfigManager::getCoinPoints()
{
    reloadIfNeeded();
    return coinPoints;
}

ConfigManager::Color ConfigManager::getPowerUpColorFromLua(float health)
{
    
    reloadIfNeeded();

    Color defaultColor = { 0, 255, 0 }; // Green by default

    if (!L)
    {
        std::cerr << "Error: Lua state not initialized" << std::endl;
        return defaultColor;
    }

    // Get the function
    lua_getglobal(L, "getPowerUpColor");

    // Verify the function
    if (!lua_isfunction(L, -1))
    {
        std::cerr << "Error: getPowerUpColor is not a Lua function" << std::endl;
        lua_pop(L, 1);
        return defaultColor;
    }

    // Send the parameter
    lua_pushnumber(L, health);

    // Call the function
    if (lua_pcall(L, 1, 1, 0) != 0)
    {
        std::cerr << "Error while calling getPowerUpColor: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return defaultColor;
    }

    // Verify that the result is a table
    if (!lua_istable(L, -1))
    {
        std::cerr << "Error: getPowerUpColor didn't return a table" << std::endl;
        lua_pop(L, 1);
        return defaultColor;
    }

    // Read the values
    Color result = { 0, 0, 0 };

    // Read r
    lua_getfield(L, -1, "r");
    if (lua_isnumber(L, -1))
    {
        result.r = lua_tointeger(L, -1);
    }
    else
    {
        std::cerr << "Color without range 'r', using 0" << std::endl;
    }
    lua_pop(L, 1);

    // Read g
    lua_getfield(L, -1, "g");
    if (lua_isnumber(L, -1))
    {
        result.g = lua_tointeger(L, -1);
    }
    else
    {
        std::cerr << "Color without range 'g', using 0" << std::endl;
    }
    lua_pop(L, 1);

    // Read b
    lua_getfield(L, -1, "b");
    if (lua_isnumber(L, -1))
    {
        result.b = lua_tointeger(L, -1);
    }
    else
    {
        std::cerr << "Color without range 'b', using 0" << std::endl;
    }
    lua_pop(L, 1);

    // Clean the table from the stack
    lua_pop(L, 1);

    return result;
}