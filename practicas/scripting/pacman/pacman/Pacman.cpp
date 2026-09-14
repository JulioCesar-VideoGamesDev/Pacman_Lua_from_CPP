#include "Pacman.h"

#include "engine_api.h"

Pacman::Pacman()
    : lives(1.5f), maxLives(1.5f), coins(0)
{
}

void Pacman::setSpeedMultiplier(float mult) {
    ::setPacmanSpeedMultiplier(mult); // CALLING THE EXTERN FROM pacman_include.cpp
}

void Pacman::setColor(int r, int g, int b, int a) {
    ::setPacmanColor((unsigned char)r, (unsigned char)g,
        (unsigned char)b, (unsigned char)a); // CALLING THE EXTERN FROM pacman_include.cpp
}

void Pacman::setPowerUpTime(int seconds) {
    ::setPowerUpTime(seconds); // CALLING THE EXTERN FROM pacman_include.cpp
}

void Pacman::setMaxLives(float l) {
    maxLives = l;
    lives = l;
}

void Pacman::loseLife(float amount) {
    lives -= amount;
    if (lives < 0.0f) lives = 0.0f;
}

void Pacman::reset() {
    lives = maxLives;
    coins = 0;
}