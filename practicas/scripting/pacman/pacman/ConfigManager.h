#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

#include "lua.hpp"

class ConfigManager {
private:

    int powerUpScore;
    float powerUpDuration;
    float powerUpSpeedMultiplier;
    
    int bronzeMedalPoints;
    int coinPoints;

    lua_State* L;

public:

    struct Color {
        int r, g, b;
    } pacmanPowerUpColor;

private:

    // For the hot-reload
    std::string configFilename;
    time_t lastFileModTime;
    bool needsReload;

    time_t getFileModTime(const std::string& filename);
    bool checkIfFileChanged();
    bool loadConfigInternal(const std::string& filename);

public:
    
    ConfigManager();
    ~ConfigManager();

    bool loadConfig(const std::string& filename);

    // Reload file if it has changed.
    bool reloadIfNeeded();

    // Getters para acceder a los valores
    int getPowerUpScore();
    float getPowerUpDuration();
    float getPowerUpSpeedMultiplier();
    Color getPacmanPowerUpColor();
    int getBronzeMedalPoints();
    int getCoinPoints();

    void setDefaultValues();

    

    Color getPowerUpColorFromLua(float health);
};

#endif