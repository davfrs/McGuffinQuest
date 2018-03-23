#include <iostream>
#include <memory>

#include "Game.h"
#include "Entity.h"
#include "Player.h"

using namespace std;
using namespace Inventory;

int main() {
    Game game("player", {100, 10, 4});

    cout << "success preparing test game" << endl;
    Entity enemy("enemy", {8, 60, 2});
    InventoryData& enemyInv = enemy.getInv();
    auto itemList = InventoryData::ItemPointerContainer();

    itemList.push_back(shared_ptr<Item>(new MoneyItem(5000)));
    itemList.push_back(shared_ptr<Item>(new WeaponItem("axe1", 1, 10)));
    itemList.push_back(shared_ptr<Item>(new WeaponItem("axe2", 2, 50)));
    itemList.push_back(shared_ptr<Item>(new ArmorItem("paper armor", 1, 50)));
    itemList.push_back(shared_ptr<Item>(new ArmorItem("iron armor", 2, 200)));

    enemyInv.addAsPossible(itemList);
    cout << enemy.getInv() << endl;

    cout << "enemy set up" << endl << endl;
    itemList.clear();
    auto potionLoc = game.consumables.find(POTION_NAME);
    if (potionLoc != game.consumables.end()) {
        auto potion = potionLoc->second;
        potion->setUses(2);
        itemList.push_back(potion);
    }

    auto& playerinv = game.player.getInv();
    playerinv.addAsPossible(itemList);
    cout << playerinv << endl;
    cout << "player inventory set up" << endl << endl;

    cout << game.player.getCurrentHP() << endl;
    enemy.attackEntity(game.player);
    cout << game.player.getCurrentHP() << endl;
    cout << playerinv.useConsumable(0) << endl;
    playerinv.printConsumables(cout);
    cout << playerinv.useConsumable(0) << endl;
    playerinv.printConsumables(cout);
    cout << "end of consumables" << endl;
    cout << game.player.getCurrentHP() << endl;

    playerinv.addAsPossible(enemy.getInv().dropEverything());
    cout << playerinv << endl;
    playerinv.printWeapons(cout);
    playerinv.printArmors(cout);
    playerinv.printConsumables(cout);
    cin.get();
}