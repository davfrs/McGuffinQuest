#ifndef PLAYER
#define PLAYER

#include "Inventory/Inventory.h"
#include "Entity.h"

class Player : public Entity {
public:
    Player(std::string name, ENTITYSTATS stats) : Entity(name, stats) { }

};


#endif