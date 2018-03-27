#ifndef GAME
#define GAME

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "Consumables.h"
#include "Map/Map.h"
#include "Entity.h"
#include "Player.h"

const std::string CHEATMODE_NAME("Joe Bentley");

class Game {
    
    std::vector<std::string> enemyNames;
    std::vector<std::string> weaponNames;
    std::vector<std::string> armorNames;
    std::map<const std::string, std::shared_ptr<Inventory::ConsumableItem>> consumables;
    bool cheatMode;
public:
    Player player;
    Map map;

    Game(std::string playerName, EntityStats playerStats);

    virtual ~Game() {
    }
    std::shared_ptr<Inventory::ConsumableItem> getConsumableItem(const std::string name);
    std::shared_ptr<Entity> generateRandomEnemy(int floorLevel);
    std::shared_ptr<Inventory::ArmorItem> generateArmor(int power);
    std::shared_ptr<Inventory::WeaponItem> generateWeapon(int power);
};

#endif