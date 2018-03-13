#include "Consumables.h"

template<typename Game>//as a workaround for both Game and Consumables using one another
Consumables<Game>::Consumables(Game& game) {
	Inventory::ConsumableItem& torch(Inventory::ConsumableItem_Lambda({}, TORCHNAME));
	//this->insert(TORCHNAME, torch);
};
