//Terrain.h
//Offers a complete suite of map stuff.

#ifndef TERRAIN
#define TERRAIN

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "dungeonbytes.h"

enum DIMENSIONS
{
    MAP_X_SIZE = 64,
    MAP_Y_SIZE = 22,
    MAP_Z_SIZE = 8
};

struct COORDINATE2
{
    int XCo;
    int YCo;
};

class COORDINATE3
{
private:
    int x;
    int y;
    int z;

public:
    COORDINATE3(int x, int y, int z) : x(x), y(y), z(z)
    {
#ifdef WRAPPING_LEVELS
        this->fixWrapping();
#endif
    }

    COORDINATE3(const COORDINATE3 &copy) = default;

    COORDINATE3 &operator=(const COORDINATE3 &other) = default;

    inline int X()
    { return x; }

    inline int Y()
    { return y; }

    inline int Z()
    { return z; }

    COORDINATE3 incrementX()
    { return COORDINATE3(this->x + 1, this->y, this->z); }

    COORDINATE3 decrementX()
    { return COORDINATE3(this->x - 1, this->y, this->z); }

    COORDINATE3 incrementY()
    { return COORDINATE3(this->x, this->y + 1, this->z); }

    COORDINATE3 decrementY()
    { return COORDINATE3(this->x, this->y - 1, this->z); }

#ifdef WRAPPING_LEVELS
    COORDINATE3 fixWrapping() {
        this->x = (this->x + MAP_X_SIZE) % MAP_X_SIZE;
        this->y = (this->y + MAP_Y_SIZE) % MAP_Y_SIZE;
        this->z = (this->z + MAP_Z_SIZE) % MAP_Z_SIZE;
    }
#endif

    bool isValid()
    {
#ifdef WRAPPING_LEVELS
        return true;

#else
        return this->x < MAP_X_SIZE && this->x > -1 &&
               this->y < MAP_Y_SIZE && this->y > -1 &&
               this->z < MAP_Z_SIZE && this->z > -1;
#endif
    }
};

class Map
{
    unsigned char dungeon[MAP_X_SIZE][MAP_Y_SIZE][MAP_Z_SIZE];

    //I'd love to make it where like the UI goes on the left
    //and then the rest of the stuff happens
    //"Str: NN" is only 7...
    //"HP: NN / XX" however is 11. So therefore absolute max width is 68 (due to needing a padding space)
    //And the inventory design could be simply like
    //ItemName (vertical), I forget how we were doing it.
    bool generateLevel(int depth);

public:
    Map()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        generateLevel(playerLocation().Z());
    }

    int revealSquare(int Xsp, int Ysp, int Zsp)
    {
        if ((dungeon[Xsp][Ysp][Zsp] & UNSEEN_TILE) == UNSEEN_TILE)
            dungeon[Xsp][Ysp][Zsp] = dungeon[Xsp][Ysp][Zsp] ^ UNSEEN_TILE;
        //strip unseen tile bit
        return dungeon[Xsp][Ysp][Zsp];
    }

    int getTilePlayer(int Xsp, int Ysp, int Zsp)
    {
    }

    bool isSquareRevealed(COORDINATE3 coord)
    {
        return (dungeon[coord.X()][coord.Y()][coord.Z()] & UNSEEN_TILE) == UNSEEN_TILE;
    }

	int revealSquare_Coord3(COORDINATE3 coord)
	{
		return revealSquare(coord.X(), coord.Y(), coord.Z());
	}

    COORDINATE3 playerLocation() {
        // TODO: keep track of player's location
        return COORDINATE3(0,0,0);
    }
};

#endif

