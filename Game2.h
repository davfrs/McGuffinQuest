#pragma once

#ifndef GAME2
#define GAME2

#include <string>

#include "Entity.h"
#include "Game.h"
#include "Consumables.h"
#include "Consumables2.h"

using std::string;

class Game2 : public Game{
public:
	Game2(string playername, EntityStats playerStats) : Game(playername, playerStats){
		this->allConsumables = new Consumables2(*this);
	}
};

#endif