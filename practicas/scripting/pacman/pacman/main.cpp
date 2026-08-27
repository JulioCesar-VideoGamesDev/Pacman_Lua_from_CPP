#include <pacman_include.hpp>

#include "ConfigManager.h"

ConfigManager g_config;

int num_coins = 0;
const int platas_para_oro = 5;
const int bronces_para_plata = 100;

const float max_vida = 1.5f;
float vida = max_vida;

bool pacmanEatenCallback(int& score, bool& muerto)
{ // Pacman ha sido comido por un fantasma
	vida -= 0.5f;
	muerto = vida < 0.0f;

	return true;
}

bool coinEatenCallback(int& score)
{ // Pacman se ha comido una moneda
	++num_coins;
	score = num_coins * g_config.getCoinPoints();

	return true;
}

bool frameCallback(float time)
{ // Se llama periodicamente cada frame
	return false;
}

bool ghostEatenCallback(int& score)
{ // Pacman se ha comido un fantasma
	return false;
}

bool powerUpEatenCallback(int& score)
{ // Pacman se ha comido un powerUp
	// Get the color based on the life.
	ConfigManager::Color color = g_config.getPowerUpColorFromLua(vida);
	setPacmanColor(color.r, color.g, color.b);

	setPacmanSpeedMultiplier(g_config.getPowerUpSpeedMultiplier());

	setPowerUpTime(g_config.getPowerUpDuration());

	score += g_config.getPowerUpScore();

	return true;
}

bool powerUpGone()
{ // El powerUp se ha acabado
	setPacmanColor(255, 0, 0);
	setPacmanSpeedMultiplier(1.0f);
	return true;
}

bool pacmanRestarted(int& score)
{
	score = 0;
	num_coins = 0;
	vida = max_vida;

	return true;
}

bool computeMedals(int& oro, int& plata, int& bronce, int score)
{
	int bronce_medal_points = g_config.getBronzeMedalPoints();

	plata = score / bronce_medal_points;
	bronce = score % bronce_medal_points;

	oro = plata / bronce_medal_points;
	plata = plata % bronce_medal_points;

	return true;
}

bool getLives(float& vidas)
{
	vidas = vida;
	return true;
}

bool setImmuneCallback()
{
	return true;
}

bool removeImmuneCallback()
{
	return true;
}

bool InitGame()
{
	if (!g_config.loadConfig("config.lua")) {
		std::cout << "Error cargando config.lua, usando valores por defecto" << std::endl;
	}

	return true;
}

bool EndGame()
{
	return true;
}