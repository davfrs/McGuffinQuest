#pragma once

#ifndef CONSUMABLES
#define CONSUMABLES


#include <map>
#include "Inventory.h"
using namespace std;
using MapSCI = map<const string, shared_ptr<Inventory::ConsumableItem>>;

const string TORCHNAME("torch");
const string POTION("potion");
const int POTIONHEAL = 10;


class Consumables : public MapSCI {
public:
	Consumables() : MapSCI() {}
	shared_ptr<Inventory::ConsumableItem>& getAndCopy(const string name) { return this->find(name)->second.get()->clone(); }
};

#endif