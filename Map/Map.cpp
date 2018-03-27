#include "Map.h"
#include <string>

//Returns false if generation failed.
bool Map::generateLevel(int dLv, bool wrappingEnabled) {
    if (dLv > MAP_Z_SIZE) {
        return false;
    }
    if (generated[dLv])
        return true;
    generated[dLv] = true;
    for (int i = 0; i < MAP_X_SIZE; i++) {
        for (int j = 0; j < MAP_Y_SIZE; j++) {
            dungeon[i][j][dLv] = MONSTER | UNSEEN_TILE;
        }
    }
    COORDINATE2 PS = this->playerLocation().toCOORDINATE2();
    COORDINATE2 PU, PD;
    PU.XCo = PU.YCo = PD.XCo = PD.YCo = 0;
    while (PU == PD || PS == PU) {
        PU.XCo = rand() % MAP_X_SIZE;
        PU.YCo = rand() % MAP_Y_SIZE;
        if (dLv == 0) {
            PD.XCo = rand() % MAP_X_SIZE;
            PD.YCo = rand() % MAP_Y_SIZE;
        } else {
            PD = PS;
        }
    }

    int t;
    bool connect = false;
    char spot = '.';
    if (dLv != MAP_Z_SIZE) {
        dungeon[PD.XCo][PD.YCo][dLv] = GO_DEEPER | UNSEEN_TILE;
    }
    else {
        dungeon[PD.XCo][PD.YCo][dLv] = GOALPOINT | UNSEEN_TILE;
    }
    if (dLv != 0) {
        dungeon[PU.XCo][PU.YCo][dLv] = GO_HIGHER;
    }
    /*
    int cX;
    int cY;
    cX = PU.XCo;
    cY = PU.YCo;
    while (!connect) {
        if (cX == PD.XCo && cY == PD.YCo)
            connect = true; //we made it~
        else {//(cX != PU.XCo || cY != PU.YCo)
            //we didn't make it yet, so set old point to floor-type and move a space.
            dungeon[cX][cY][dLv] = FLOOR;
            t = rand() % 4;
            switch (t) {
            case 0:
                cX++;
                break;
            case 1:
                cX--;
                break;
            case 2:
                cY++;
                break;
            case 3:
                cY--;
                break;
            }
            if (wrappingEnabled) {
                if (cX == -1) {
                    cX = MAP_X_SIZE - 1;
                }
                if (cY == -1) {
                    cY = MAP_Y_SIZE - 1;
                }
                if (cX == MAP_X_SIZE) {
                    cX = 0;
                }
                if (cY == MAP_Y_SIZE) {
                    cY = 0;
                }
            }
            else {
                if (cX == -1) {
                    cX = 0;
                }
                if (cY == -1) {
                    cY = 0;
                }
                if (cX == MAP_X_SIZE) {
                    cX = MAP_X_SIZE - 1;
                }
                if (cY == MAP_Y_SIZE) {
                    cY = MAP_Y_SIZE - 1;
                }
            }
        }
    }

    //ie: we now have a connection between PD and PU!
    for (int i = 0; i < MAP_X_SIZE; i++) {
        for (int j = 0; j < MAP_Y_SIZE; j++) {
            if (dungeon[i][j][dLv] == FLOOR) {
                t = rand() % 7;
            }
            else {
                t = 7;
            }
            if (t >= 6) {
                t = rand() % RANDOMIZABLE_TERRAIN_TYPES;
                switch (t) {
                    //If you want to add more terrain, you have to do it in two places.
                    //Here, and its sister (which has the same comment).
                case 0:
                    spot = MONSTER | UNSEEN_TILE;
                    break;
                case 1:
                    spot = MERCHANT | UNSEEN_TILE;
                    break;
                case 2:
                    spot = WARP | UNSEEN_TILE;
                    break;
                case 3:
                    spot = TRAP | UNSEEN_TILE;
                    break;
                case 4:
                    spot = LOOT | UNSEEN_TILE;
                    break;
                case 5:
                    spot = STATUE | UNSEEN_TILE;
                    break;
                case 6:
                    spot = HEAL | UNSEEN_TILE;
                    break;
                }
                dungeon[i][j][dLv] = spot;
            }
        }
    }*/
    std::string randomTiles;
    randomTiles += std::string(10, MONSTER | UNSEEN_TILE);
    //randomTiles += std::string(2, MERCHANT | UNSEEN_TILE);
    //randomTiles += std::string(1, WARP | UNSEEN_TILE);
    //randomTiles += std::string(5, TRAP | UNSEEN_TILE);
    //randomTiles += std::string(1, LOOT | UNSEEN_TILE);
    //randomTiles += std::string(1, STATUE | UNSEEN_TILE);
    //randomTiles += std::string(1, HEAL | UNSEEN_TILE);
    randomTiles += std::string(15, FLOOR | UNSEEN_TILE);
    const int tiles = randomTiles.size();
    for (int i = 0; i < MAP_X_SIZE; i++) {
        for (int j = 0; j < MAP_Y_SIZE; j++) {
            if(dungeon[i][j][dLv] == (MONSTER | UNSEEN_TILE))
            dungeon[i][j][dLv] = randomTiles[rand() % tiles];
        }
    }
    return true;
}