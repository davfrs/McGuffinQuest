#pragma once

#ifndef CONSUMABLES
#define CONSUMABLES


#include <map>
#include "Inventory.h"
using namespace std;
using MapSCI = map<const string, reference_wrapper<Inventory::ConsumableItem>>;

const string TORCHNAME("torch");

template<typename Game>//as a workaround for both Game and Consumables using one another
class Consumables : MapSCI {
public:
	Consumables(Game& game);
	virtual ~Consumables() {}

};

#endif