// engine_api.h
#pragma once
#ifndef ENGINE_API_H
#define ENGINE_API_H

// Copy of the declaration from pacman_include.hpp without the app
extern void setPacmanSpeedMultiplier(float speed);
extern void setPacmanColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
extern void setPowerUpTime(int seconds);

#endif