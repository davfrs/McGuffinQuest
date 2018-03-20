#pragma once

#ifndef PLAYER
#define PLAYER

#include "Inventory.h"
#include "Terrain.h"
#include "Entity.h"

class Player : public Entity{
public:

	
	Player(string name, EntityStats stats) :Entity(name, stats) { }
};

#endif