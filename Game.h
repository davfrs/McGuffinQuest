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

class Game {
    std::vector<std::string> enemyNames;
    std::vector<std::string> weaponNames;
    std::vector<std::string> armorNames;
    std::map<const std::string, std::shared_ptr<Inventory::ConsumableItem>> consumables;
public:
    Player player;
    Map map;

    Game(std::string playerName, EntityStats playerStats);

    virtual ~Game() {
    }
    std::shared_ptr<Inventory::ConsumableItem> getConsumableItem(const std::string name);
    std::shared_ptr<Entity> generateRandomEnemy(int floorLevel);
};

#endif