#ifndef GAME
#define GAME

#include <string>

#include "Consumables.h"
#include "Map/Map.h"
#include "Entity.h"
#include "Player.h"

class Game {

public:
    Player player;
    Map map;

    std::map<const std::string, std::shared_ptr<Inventory::ConsumableItem>> consumables;

    Game(std::string playerName, EntityStats playerStats);

    virtual ~Game() {
    }
};

#endif