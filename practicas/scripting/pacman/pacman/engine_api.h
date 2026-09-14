// engine_api.h
#pragma once
#ifndef ENGINE_API_H
#define ENGINE_API_H

// Declaraciones de la API del motor (copia de pacman_include.hpp SIN la línea de app)
extern void setPacmanSpeedMultiplier(float speed);
extern void setPacmanColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
extern void setPowerUpTime(int seconds);

#endif