#pragma once

#ifndef GAME
#define GAME

class Game;

#include "Terrain.h"
#include "Player.h"
#include "Consumables.h"

#define INVENTORYLIMIT 8

class Game {
	
public:
	PlayerData player;
	Map map;


	Consumables* allConsumables;
	Game() : player(), map(){
		allConsumables = new Consumables();
	}
	~Game() {
		delete allConsumables;
	}
};

#endif