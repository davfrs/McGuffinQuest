#pragma once

#ifndef GAME
#define GAME

class Game;

#include <string>

#include "Terrain.h"
#include "Entity.h"
#include "Player.h"
#include "Consumables.h"

using std::string;

#define INVENTORYLIMIT 8

class Game {
	
public:
	Player player;
	Map map;


	Consumables* allConsumables;
	Game(string playername, EntityStats playerStats) : player(playername, playerStats), map(), allConsumables(nullptr) {
		
	}
	virtual ~Game() {
		if (allConsumables)
			delete allConsumables;
	}
};

#endif