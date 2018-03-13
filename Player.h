#pragma once

#ifndef PLAYER
#define PLAYER

#include "Inventory.h"
#include "Terrain.h"

#define INVENTORY_CAPACITY 16
class PlayerData {
public://just for now, to get things together
	Inventory::Inventory inv;
	COORDINATE3 location;
	PlayerData() : inv(INVENTORY_CAPACITY), location(0,0,0) { }
};

#endif