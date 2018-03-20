
#include <iostream>

#include "Game2.h"
#include "Entity.h"
#include "Player.h"

int main() {
	Game& game(Game2("player",{100, 10, 4}));
	
	Entity enemy("enemy",{ 100, 8, 2 });
	
	cout << "success preparing test" << endl;
	cout << enemy.getCurrentHP() << endl;
	game.player.attackEntity(enemy);//player has 10 base attack, enemy has 2 base defense. expecting 10-2=8 damage.
	cout << enemy.getCurrentHP() << endl;

	cin.get();
}