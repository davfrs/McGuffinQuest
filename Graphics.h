#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <iostream>
#include <string>

#include "Map/Map.h"

enum VIEW {
    VIEW_MAP,
    VIEW_INVENTORY
};

class Graphics {
private:
    Map map{};

    void redrawMap();

    std::string printInventory(int line);

public:
    void show(VIEW view);
};

#endif //PROJECT_GRAPHICS_H
