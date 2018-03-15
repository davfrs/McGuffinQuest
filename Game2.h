#pragma once

#ifndef GAME2
#define GAME2

#include "Game.h"
#include "Consumables.h"
#include "Consumables2.h"

class Game2 : public Game{
public:
	Game2() : Game(){
		this->allConsumables = new Consumables2(*this);
	}
};

#endif