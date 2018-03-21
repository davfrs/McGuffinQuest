#pragma once

#ifndef CONSUMABLES
#define CONSUMABLES

#include <string>
#include <map>
#include <vector>

#include "Inventory.h"
using namespace std;
using MapSCI = map<const string, shared_ptr<Inventory::ConsumableItem>>;

const string TORCH_NAME("torch");
const string POTION_NAME("potion");
const string MAXPOTION_NAME("maxpotion");



const string TORCH_DESC("Exposes nearby tiles");
const int TORCH_WORTH(50);

const int POTION_HEAL = 10;
const string POTION_DESC("Heals up to 10 HP");
const int POTION_WORTH(50);

const string MAXPOTION_DESC("Fully heals you");
const int MAXPOTION_WORTH(500);


class Consumables : public MapSCI {
public:
	Consumables() : MapSCI() {}
	shared_ptr<Inventory::ConsumableItem> getAndCopy(const string name) { return this->find(name)->second.get()->clone(); }
};

#endif