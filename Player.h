#pragma once

#ifndef PLAYER
#define PLAYER

#include "Inventory.h"
#include "Terrain.h"
#include "Entity.h"

class Player : public Entity{
public:
    Player(EntityStats stats) :Entity(stats) { }
    int Status();
    bool moveUp();
    bool moveDown();
    bool moveNorth();
    bool moveSouth();
    bool moveEast();
    bool moveWest();
};

int Player::Status() {
    //Win condition
    if (getCurrentHP() != 0)
        return 1;
    else 
        return 0;
}

/*  unsigned char getTile(COORDINATE3 coord)
    {
        return dungeon[coord.X()][coord.Y()][coord.Z()];
    };*/

bool Player::moveUp() { 
    if (map.getTile(map.playerSpace) == '<') {
        if (map.playerSpace.Z() == 0) {
            if (Status() == 2) {
                //handle victory
            }
        }
        else if (map.playerSpace.Z() > 0) {
            map.updatePlayer(map.playerSpace.X(), map.playerSpace.Y(), map.playerSpace.Z()-1);
    }
}

bool Player::moveDown() { 
    if (map.getTile(map.playerSpace) == '>') {
        if (map.playerSpace.Z() > 0) {
            map.updatePlayer(map.playerSpace.X(), map.playerSpace.Y(), map.playerSpace.Z()+1);
    }

}

bool Player::moveNorth() { 
    
}

bool Player::moveSouth() { 

}

bool Player::moveEast() { 

}

bool Player::moveWest() { 

}
