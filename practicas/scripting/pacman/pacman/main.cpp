#include <pacman_include.hpp>

#include "LuaManager.h"

//LuaManager g_lua;

#include "lua.hpp"
#include "Pacman.h"


// Constantes que siguen viviendo en C++ (puntuación, medallas)
const int platas_para_oro = 5;
const int bronces_para_plata = 100;

// ---------- Helpers para acceder a la instancia Pacman de Lua ----------
static Pacman* getPacman() {
	lua_State* L = LuaManager::instance().getLuaState();
	if (!L) return nullptr;
	lua_getglobal(L, "pacman");
	Pacman* p = (Pacman*)luaL_checkudata(L, -1, "Pacman");
	lua_pop(L, 1);
	return p;
}

// CALLBACKS -------------------------------------------------------------------------

bool pacmanEatenCallback(int& score, bool& muerto)
{
	Pacman* p = getPacman();
	if (!p) { muerto = true; return false; }

	p->loseLife(0.5f);
	muerto = p->getLives() <= 0.0f;
	return true;
}

bool coinEatenCallback(int& score)
{
	Pacman* p = getPacman();
	if (!p) return false;

	p->addCoin();

	// Leemos coinPoints desde Lua directamente para no depender de ConfigManager
	lua_State* L = LuaManager::instance().getLuaState();
	lua_getglobal(L, "coinPoints");
	int coinPoints = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	score = p->getCoins() * coinPoints;
	return true;
}

static bool configLoaded = false;

bool frameCallback(float time)
{
	if (!configLoaded) {
		if (!LuaManager::instance().init()) {
			std::cout << "Error inicializando Lua" << std::endl;

			return false;
		}

		if (!LuaManager::instance().loadConfig("config.lua")) {
			std::cout << "Error cargando config.lua" << std::endl;
		}
		configLoaded = true;
		// Después de cargar, aplicamos también lo que queramos del script
	}
	else
	{
		LuaManager::instance().reloadIfNeeded();
	}

	return false;
}

bool ghostEatenCallback(int& score)
{
	return false;
}

bool powerUpEatenCallback(int& score)
{
	Pacman* p = getPacman();
	if (!p) return false;

	lua_State* L = LuaManager::instance().getLuaState();
	if (!L) return false;

	// Color según la vida (función Lua)
	lua_getglobal(L, "getPowerUpColor");
	lua_pushnumber(L, p->getLives());
	if (lua_pcall(L, 1, 1, 0) == 0 && lua_istable(L, -1)) {
		lua_getfield(L, -1, "r"); int r = (int)lua_tointeger(L, -1); lua_pop(L, 1);
		lua_getfield(L, -1, "g"); int g = (int)lua_tointeger(L, -1); lua_pop(L, 1);
		lua_getfield(L, -1, "b"); int b = (int)lua_tointeger(L, -1); lua_pop(L, 1);
		setPacmanColor((unsigned char)r, (unsigned char)g, (unsigned char)b, 255);
	}
	else {
		std::cerr << "getPowerUpColor failed" << std::endl;
	}
	lua_pop(L, 1);

	// Multiplicador y duración del powerUp
	lua_getglobal(L, "powerUpSpeedMultiplier");
	float mult = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	setPacmanSpeedMultiplier(mult);

	lua_getglobal(L, "powerUpDuration");
	int dur = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
	setPowerUpTime(dur);

	// Puntos
	lua_getglobal(L, "powerUpScore");
	int powerScore = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	score += powerScore;

	return true;
}

bool powerUpGone()
{
	setPacmanColor(255, 0, 0);
	setPacmanSpeedMultiplier(1.0f);
	return true;
}

bool pacmanRestarted(int& score)
{
	Pacman* p = getPacman();
	if (p) p->reset();
	score = 0;
	return true;
}

bool computeMedals(int& oro, int& plata, int& bronce, int score)
{
	lua_State* L = LuaManager::instance().getLuaState();
	if (!L) return false;

	lua_getglobal(L, "bronzeMedalPoints");
	int bronce_medal_points = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	if (bronce_medal_points <= 0) bronce_medal_points = 500;

	plata = score / bronce_medal_points;
	bronce = score % bronce_medal_points;
	oro = plata / bronce_medal_points;
	plata = plata % bronce_medal_points;
	return true;
}

bool getLives(float& vidas)
{
	Pacman* p = getPacman();
	if (!p) { vidas = 0.0f; return false; }
	vidas = p->getLives();
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
	return true;
}

bool EndGame()
{
	return true;
}