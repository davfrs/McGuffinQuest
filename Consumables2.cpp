#include "Consumables2.h"
#include "Game.h"
#include "Terrain.h"

bool torchLogic(Game& game) {
	COORDINATE3 location = game.map.playerLocation();
	Map map = game.map;
	bool activated = false;

	COORDINATE3 t = location.incrementX();
	if (t.isValid()) {
		if (map.isSquareRevealed(t)) {
			map.revealSquare_Coord3(t);
			activated = true;
		}
	}

	t = location.decrementX();
	if (t.isValid()) {
		if (map.isSquareRevealed(t)) {
			map.revealSquare_Coord3(t);
			activated = true;
		}
	}

	t = location.incrementY();
	if (t.isValid()) {
		if (map.isSquareRevealed(t)) {
			map.revealSquare_Coord3(t);
			activated = true;
		}
	}

	t = location.decrementY();
	if (t.isValid()) {
		if (map.isSquareRevealed(t)) {
			map.revealSquare_Coord3(t);
			activated = true;
		}
	}
	
	return activated;
}
Consumables2::Consumables2(Game& game) {
	//torch
	this->emplace(TORCH_NAME, (*new Inventory::ConsumableItem_Lambda([&game]() {return torchLogic(game);}, TORCH_NAME, TORCH_DESC, TORCH_WORTH))());
	this->emplace(POTION_NAME, (*new Inventory::ConsumableItem_Lambda([&game]() {return game.player.heal(POTION_HEAL);}, POTION_NAME, POTION_DESC, POTION_WORTH))());
	this->emplace(MAXPOTION_NAME, (*new Inventory::ConsumableItem_Lambda([&game]() {return game.player.heal(game.player.getMaxHP());}, MAXPOTION_NAME, MAXPOTION_DESC, MAXPOTION_WORTH))());
};