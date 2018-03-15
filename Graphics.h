#ifndef PROJECT_GRAPHICS_H
#define PROJECT_GRAPHICS_H

#include <iostream>
#include <string>

#include "Terrain.h"

enum VIEW
{
    VIEW_MAP,
    VIEW_INVENTORY
};

class Graphics
{
private:
    Map map{};
    void redrawMap();

public:
    void show(VIEW view);

};

#endif //PROJECT_GRAPHICS_H
