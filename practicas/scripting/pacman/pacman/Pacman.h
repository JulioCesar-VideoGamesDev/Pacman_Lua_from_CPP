#pragma once
#ifndef PACMAN_H
#define PACMAN_H

class Pacman {
public:
    Pacman();

    // Setters that use the engine library.
    void setSpeedMultiplier(float mult);
    void setColor(int r, int g, int b, int a = 255);
    void setPowerUpTime(int seconds);

    // State of the player
    void  setMaxLives(float l);
    float getMaxLives() const { return maxLives; }

    float getLives() const { return lives; }
    void  setLives(float l) { lives = l; }
    void  loseLife(float amount);

    int   getCoins() const { return coins; }
    void  setCoins(int c) { coins = c; }
    void  addCoin() { ++coins; }

    void  reset();

private:
    float lives;
    float maxLives;
    int   coins;
};

#endif