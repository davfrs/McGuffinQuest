#ifndef PLAYER
#define PLAYER

#include "Inventory/Inventory.h"
#include "Terrain.h"
#include "Entity.h"
#include "Map/Map.h"

class Player : public Entity {
public:
    Player(std::string name, EntityStats stats) : Entity(name, stats) {
    }

    int Status();


};


#endif