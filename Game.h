#pragma once

#ifndef GAME
#define GAME

class Game;

#include "Terrain.h"
#include "Entity.h"
#include "Player.h"
#include "Consumables.h"

#define INVENTORYLIMIT 8

class Game {
	
public:
	Player player;
	Map map;


	Consumables* allConsumables;
	Game(EntityStats playerStats) : player(playerStats), map(), allConsumables(nullptr){
		
	}
	virtual ~Game() {
		if (allConsumables)
			delete allConsumables;
	}
};

#endif