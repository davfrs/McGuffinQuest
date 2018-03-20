#pragma once

#ifndef ENTITY
#define ENTITY

#include <string>
#include "Inventory.h"
#include "Terrain.h"

#define STARTING_INVENTORY_CAPACITY 8

using std::string;

struct EntityStats {
	int HP;
	int strength;
	int defense;
};

class Entity {
	string name;
	EntityStats original, active;
	Inventory::Inventory inv;
	bool takeExactDamage(int damage);
protected:
	bool defendFromAttack(int damage);
public:
	Entity(string name, EntityStats stats) : name(name), original(stats), active(stats), inv(STARTING_INVENTORY_CAPACITY) {}
	void resetActiveStats();
	bool attackEntity(Entity& other);//returns true if the target is now dead
	
	const string getName() const { return this->name; }

	inline Inventory::Inventory& getInv() { return this->inv; }
	int getCurrentHP() const { return this->active.HP; }
	int getMaxHP() const { return this->original.HP; }
	bool heal(int health);

};

#endif
