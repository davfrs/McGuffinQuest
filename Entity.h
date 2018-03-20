#pragma once

#ifndef ENTITY
#define ENTITY

#include "Inventory.h"
#include "Terrain.h"

#define STARTING_INVENTORY_CAPACITY 8

struct EntityStats {
	int HP;
	int strength;
	int defense;
};

class Entity {
	EntityStats original, active;
	Inventory::Inventory inv;
	bool takeExactDamage(int damage);
protected:
	bool defendFromAttack(int damage);
public:
	Entity(EntityStats stats) : original(stats), active(stats), inv(STARTING_INVENTORY_CAPACITY) {}
	void resetActiveStats();
	bool attackEntity(Entity& other);//returns true if the target is now dead
	
	inline Inventory::Inventory& getInv() { return this->inv; }
	int getCurrentHP() { return this->active.HP; }
	int getMaxHP() { return this->original.HP; }
	void heal(int health);

};

#endif
