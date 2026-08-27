#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

class ConfigManager {
private:

    int powerUpScore;
    float powerUpDuration;
    float powerUpSpeedMultiplier;
    struct Color {
        int r, g, b;
    } pacmanPowerUpColor;
    int bronzeMedalPoints;
    int coinPoints;

public:
    ConfigManager();
    ~ConfigManager();

    bool loadConfig(const std::string& filename);

    // Getters para acceder a los valores
    int getPowerUpScore() const { return powerUpScore; }
    float getPowerUpDuration() const { return powerUpDuration; }
    float getPowerUpSpeedMultiplier() const { return powerUpSpeedMultiplier; }
    Color getPacmanPowerUpColor() const { return pacmanPowerUpColor; }
    int getBronzeMedalPoints() const { return bronzeMedalPoints; }
    int getCoinPoints() const { return coinPoints; }

    void setDefaultValues();
};

#endif