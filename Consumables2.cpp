#include "Consumables2.h"
#include "Game.h"
#include "Terrain.h"

bool torchLogic(Game& game) {
	COORDINATE3 location(game.player.location);
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
	//this->emplace(TORCHNAME, (Inventory::ConsumableItem_Lambda([&game]() {return torchLogic(game);}, TORCHNAME)));
};