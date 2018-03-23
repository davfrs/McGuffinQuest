#ifndef GAME
#define GAME

class Game;

#include <string>
#include <utility>

#include "Terrain.h"
#include "Entity.h"
#include "Player.h"
#include "Consumables.h"

using std::string;

class Game {

public:
    Player player;
    Map map;

    Consumables *allConsumables;

    Game(string playerName, EntityStats playerStats) : player(std::move(playerName), playerStats), map(), allConsumables(nullptr) {

    }

    virtual ~Game() {
        if (allConsumables)
            delete allConsumables;
    }
};

#endif