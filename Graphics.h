#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <iostream>
#include <string>

#include "Game.h"
#include "Map/Map.h"
 
enum VIEW {
    VIEW_MAP,
    VIEW_INVENTORY
};

class Graphics {
private:
    Game& game;

    void redrawMap();

    std::string printInventory(int line);

public:
    Graphics(Game& game) : game(game) {}
    void show(VIEW view);
};

#endif //PROJECT_GRAPHICS_H
