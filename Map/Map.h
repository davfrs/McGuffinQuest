//Terrain.h
//Offers a complete suite of map stuff.

#ifndef TERRAIN
#define TERRAIN

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "../dungeonbytes.h"

enum DIMENSIONS {
    MAP_X_SIZE = 64,
    MAP_Y_SIZE = 22,
    MAP_Z_SIZE = 8
};

struct COORDINATE2 {
    int XCo, YCo;
    bool operator==(const COORDINATE2& other) {
        return XCo == other.XCo && YCo == other.YCo;
    }
};
class COORDINATE3 {
private:
    int x;
    int y;
    int z;

public:
    COORDINATE3(int x, int y, int z) : x(x), y(y), z(z) {
#ifdef WRAPPING_LEVELS
        this->fixWrapping();
#endif
    }

    COORDINATE3(const COORDINATE3& copy) = default;

    COORDINATE3& operator=(const COORDINATE3& other) = default;
    bool operator==(const COORDINATE3& other) {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const COORDINATE3& other) {
        return x != other.x || y != other.y || z != other.z;
    }
    inline int X() {
        return x;
    }
    inline int Y() {
        return y;
    }
    inline int Z() {
        return z;
    }
    COORDINATE2 toCOORDINATE2() {
        COORDINATE2 ret;
        ret.XCo = x;
        ret.YCo = y;
        return ret;
    }
    COORDINATE3 incrementX() {
        return COORDINATE3(this->x + 1, this->y, this->z);
    }
    COORDINATE3 decrementX() {
        return COORDINATE3(this->x - 1, this->y, this->z);
    }
    COORDINATE3 incrementY() {
        return COORDINATE3(this->x, this->y + 1, this->z);
    }
    COORDINATE3 decrementY() {
        return COORDINATE3(this->x, this->y - 1, this->z);
    }
    COORDINATE3 incrementZ() {
        return COORDINATE3(this->x, this->y, this->z + 1);
    }
    COORDINATE3 decrementZ() {
        return COORDINATE3(this->x, this->y, this->z - 1);
    }

#ifdef WRAPPING_LEVELS
    COORDINATE3 fixWrapping() {
        this->x = (this->x + MAP_X_SIZE) % MAP_X_SIZE;
        this->y = (this->y + MAP_Y_SIZE) % MAP_Y_SIZE;
        this->z = (this->z + MAP_Z_SIZE) % MAP_Z_SIZE;
    }
#endif

    bool isValid() {
#ifdef WRAPPING_LEVELS
        return true;
#else
        return this->x < MAP_X_SIZE && this->x > -1 && this->y < MAP_Y_SIZE && this->y > -1 &&
               this->z < MAP_Z_SIZE && this->z > -1;
#endif
    }
};

class Map {
    unsigned char dungeon[MAP_X_SIZE][MAP_Y_SIZE][MAP_Z_SIZE];
    bool generated[MAP_Z_SIZE];

    COORDINATE3 playerSpace = COORDINATE3(MAP_X_SIZE / 2, 0, 0);

    //I'd love to make it where like the UI goes on the left
    //and then the rest of the stuff happens
    //"Str: NN" is only 7...
    //"HP: NN / XX" however is 11. So therefore absolute max width is 68 (due to needing a padding space)
    //And the inventory design could be simply like
    //ItemName (vertical), I forget how we were doing it.
    bool generateLevel(int dLv, bool wrappingEnabled = false);

public:
    Map() {
        for (int i = 0;i < MAP_Z_SIZE;i++)
            generated[i] = false;
        generateLevel(playerLocation().Z());
        this->__setTile(playerLocation(), FLOOR);
    }

    int revealSquare(int Xsp, int Ysp, int Zsp) {
        if ((dungeon[Xsp][Ysp][Zsp] & UNSEEN_TILE) == UNSEEN_TILE)
            dungeon[Xsp][Ysp][Zsp] = dungeon[Xsp][Ysp][Zsp] ^ UNSEEN_TILE;
        //strip unseen tile bit
        return dungeon[Xsp][Ysp][Zsp];
    };

    int revealSquare(COORDINATE3 coord) {
        int Xsp = coord.X();
        int Ysp = coord.Y();
        int Zsp = coord.Z();

        if ((dungeon[Xsp][Ysp][Zsp] & UNSEEN_TILE) == UNSEEN_TILE)
            dungeon[Xsp][Ysp][Zsp] = dungeon[Xsp][Ysp][Zsp] ^ UNSEEN_TILE;
        //strip unseen tile bit
        return dungeon[Xsp][Ysp][Zsp];
    };
    void __setTile(COORDINATE3 coord, unsigned char tile) {
        dungeon[coord.X()][coord.Y()][coord.Z()] = tile;
    }
    unsigned char getTile(int Xsp, int Ysp, int Zsp) {
        return getTile({ Xsp, Ysp, Zsp });
    };

    unsigned char getTile(COORDINATE3 coord) {
        if (playerLocation() == coord)
            return PLAYER_TILE_CHARACTER;
        if (isSquareRevealed(coord))
            return dungeon[coord.X()][coord.Y()][coord.Z()];
        return UNSEEN_TILE_CHARACTER;
    };

    unsigned char getTilePlayer() {
        COORDINATE3 coord = this->playerLocation();
        return dungeon[coord.X()][coord.Y()][coord.Z()];
    };

    bool isSquareRevealed(COORDINATE3 coord) {
        return isSquareRevealed(coord.X(),coord.Y(),coord.Z());
    }
    bool isSquareRevealed(int Xsp, int Ysp, int Zsp) {
        return (dungeon[Xsp][Ysp][Zsp] & UNSEEN_TILE) != UNSEEN_TILE;
    }

    COORDINATE3 playerLocation() {
        return playerSpace;
    }

    unsigned char updatePlayer(COORDINATE3 newPlayerSpace) {
        return revealSquare(this->playerSpace = newPlayerSpace);
    }
    void warpToNextDeaperLevel() {
        this->playerSpace = this->playerSpace.incrementZ();
        this->generateLevel(this->playerSpace.Z());
        this->revealSquare(this->playerSpace);
    }
    void warpUpALevel() {
        this->playerSpace = this->playerSpace.decrementZ();
    }
};

#endif

