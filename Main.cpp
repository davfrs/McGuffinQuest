
#include <iostream>
#include <memory>

#include "Game2.h"
#include "Entity.h"
#include "Player.h"

using namespace Inventory;

int main() {
    {
        Game game(Game2("player", {100, 10, 4}));

        cout << "success preparing test game" << endl;
        Entity enemy("enemy", {8, 60, 2});
        InventoryData &enemyInv = enemy.getInv();
        auto itemList = InventoryData::ItemPointerContainer();

        itemList.push_back(make_shared<Item>(MoneyItem(5000)));
        itemList.push_back(make_shared<Item>(WeaponItem("axe1", 1, 10)));
        itemList.push_back(make_shared<Item>(WeaponItem("axe2", 2, 50)));
        itemList.push_back(make_shared<Item>(ArmorItem("paper armor", 1, 50)));
        itemList.push_back(make_shared<Item>(ArmorItem("iron armor", 2, 200)));

        for (auto item: itemList) {
            cout << *item << endl;
        }
        enemyInv.addAsPossible(itemList);
        cout << enemy.getInv() << endl;

        cout << "enemy set up" << endl << endl;
        itemList.clear();
        auto potion = game.allConsumables->getAndCopy(POTION_NAME);
        potion->setUses(2);
        itemList.push_back(potion);
        auto &playerinv = game.player.getInv();
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
    }
    cin.get();
}