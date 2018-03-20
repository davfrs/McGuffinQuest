
#include <iostream>

#include "Game2.h"
#include "Entity.h"
#include "Player.h"

int main() {
	Game& game(Game2("player",{100, 10, 4}));
	
	Entity enemy("enemy",{ 100, 50, 2 });
	
	cout << "success preparing test" << endl;
	cout << game.player.getCurrentHP() << endl;
	enemy.attackEntity(game.player);
	cout << game.player.getCurrentHP() << endl;
	cout << game.allConsumables->getAndCopy("potion")->attemptUse() << endl;
	cout << game.player.getCurrentHP() << endl;
	cin.get();
}