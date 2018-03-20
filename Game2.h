#pragma once

#ifndef GAME2
#define GAME2

#include "Entity.h"
#include "Game.h"
#include "Consumables.h"
#include "Consumables2.h"

class Game2 : public Game{
public:
	Game2(EntityStats playerStats) : Game(playerStats){
		this->allConsumables = new Consumables2(*this);
	}
};

#endif