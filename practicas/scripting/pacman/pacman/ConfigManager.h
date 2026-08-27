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
    ConfigManager();
    ~ConfigManager();

    bool loadConfig(const std::string& filename);

    // Getters para acceder a los valores
    int getPowerUpScore() const { return powerUpScore; }
    float getPowerUpDuration() const { return powerUpDuration; }
    float getPowerUpSpeedMultiplier() const { return powerUpSpeedMultiplier; }
    int getBronzeMedalPoints() const { return bronzeMedalPoints; }
    int getCoinPoints() const { return coinPoints; }

    void setDefaultValues();

    struct Color {
        int r, g, b;
    } pacmanPowerUpColor;

    Color getPacmanPowerUpColor() const { return pacmanPowerUpColor; }
    Color getPowerUpColorFromLua(float health);
};

#endif