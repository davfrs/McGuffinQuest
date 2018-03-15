//Terrain.h
//Offers a complete suite of map stuff.

#ifndef TERRAIN
	#define TERRAIN
	#include "dungeonbytes.h"

#ifndef MAP_X_SIZE
	#define MAP_X_SIZE 64
#endif
#ifndef MAP_Y_SIZE
	#define MAP_Y_SIZE 22
#endif
#ifndef MAP_Z_SIZE
	#define MAP_Z_SIZE 8
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct COORDINATE {
	int XCo, YCo;
};
class COORDINATE3 {
	int x, y, z;
public:
	COORDINATE3() {}
	COORDINATE3(int x, int y, int z) : x(x), y(y), z(z) {
#ifdef WRAPPING_LEVELS
		this->fixWrapping();
#endif
	}
	COORDINATE3(const COORDINATE3& copy) :x(copy.x), y(copy.y), z(copy.z) {}
	COORDINATE3& operator=(const COORDINATE3& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
	inline int X() { return x; }
	inline int Y() { return y; }
	inline int Z() { return z; }
	COORDINATE3 incrementX() { return COORDINATE3(this->x + 1, this->y, this->z); }
	COORDINATE3 decrementX() { return COORDINATE3(this->x - 1, this->y, this->z); }
	COORDINATE3 incrementY() { return COORDINATE3(this->x, this->y + 1, this->z); }
	COORDINATE3 decrementY() { return COORDINATE3(this->x, this->y - 1, this->z); }
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
		return this->x < MAP_X_SIZE && this->x > -1 &&
		       this->y < MAP_Y_SIZE && this->y > -1 &&
		       this->z < MAP_Z_SIZE && this->z > -1;
#endif
	}
};
class Map {
	unsigned char dungeon[MAP_X_SIZE][MAP_Y_SIZE][MAP_Z_SIZE];
	//I'd love to make it where like the UI goes on the left
	//and then the rest of the stuff happens
	//"Str: NN" is only 7...
	//"HP: NN / XX" however is 11. So therefore absolute max width is 68 (due to needing a padding space)
	//And the inventory design could be simply like
	//ItemName (vertical), I forget how we were doing it.
	bool generateLevel(int dLv);
public:
	int revealSquare(int Xsp, int Ysp, int Zsp) {
		if ( (dungeon[Xsp][Ysp][Zsp] & UNSEEN_TILE) == UNSEEN_TILE )
			dungeon[Xsp][Ysp][Zsp] = dungeon[Xsp][Ysp][Zsp] ^ UNSEEN_TILE;
			//strip unseen tile bit
		return dungeon[Xsp][Ysp][Zsp];
	};

	int getTilePlayer(int Xsp, int Ysp, int Zsp) {
		
	};

	bool isSquareRevealed(COORDINATE3 coord) {
		return (dungeon[coord.X()][coord.Y()][coord.Z()] & UNSEEN_TILE) == UNSEEN_TILE;
	}
	int revealSquare(COORDINATE3 coord) {
		return revealSquare(coord.X(), coord.Y(), coord.Z());
	}
};

//Returns false if generation failed.
bool Map::generateLevel(int dLv) {
	if (dLv > MAP_Z_SIZE) {
		return false;
	}
	
	COORDINATE PU, PD;
	PU.XCo = PU.YCo = PD.XCo = PD.YCo = 0;
	while (PU.XCo == PD.XCo && PU.YCo == PD.YCo) {
		PU.XCo = rand() % MAP_X_SIZE; PU.YCo = rand() % MAP_Y_SIZE;
		PD.XCo = rand() % MAP_X_SIZE; PD.YCo = rand() % MAP_Y_SIZE;
	}
	
	int t; bool connect = false; char spot = '.';
	if (dLv != MAP_Z_SIZE) {
		dungeon[PD.XCo][PD.YCo][dLv] = GO_DEEPER | UNSEEN_TILE; }
	else {
		dungeon[PD.XCo][PD.YCo][dLv] = GOALPOINT | UNSEEN_TILE; }
	dungeon[PU.XCo][PU.YCo][dLv] = GO_HIGHER;
	auto cX = PU.XCo; auto cY = PU.YCo;
	while (!connect) {
		if (cX == PD.XCo && cY == PD.YCo)
			connect = true; //we made it~
		else {//(cX != PU.XCo || cY != PU.YCo)
			//we didn't make it yet, so set old point to floor-type and move a space.
			dungeon[cX][cY][dLv] = FLOOR;
			t = rand() % 4;
			switch (t) {
				case 0: cX++; break;
				case 1: cX--; break;
				case 2: cY++; break;
				case 3: cY--; break;
			}
			#ifdef WRAPPING_LEVELS
				if (cX == -1) { cX = MAP_X_SIZE - 1; }
				if (cY == -1) { cY = MAP_Y_SIZE - 1; }
				if (cX == MAP_X_SIZE) { cX = 0; }
				if (cY == MAP_Y_SIZE) { cY = 0; }
			#else
				if (cX == -1) { cX = 0; }
				if (cY == -1) { cY = 0; }
				if (cX == MAP_X_SIZE) { cX = MAP_X_SIZE - 1; }
				if (cY == MAP_Y_SIZE) { cY = MAP_Y_SIZE - 1; }
			#endif
		} //end else
	} //end while !connect
	//ie: we now have a connection between PD and PU!
	for (int i = 0; i < MAP_X_SIZE; i++) {
		for (int j = 0; j < MAP_Y_SIZE; j++) {
			if (dungeon[i][j][dLv] == FLOOR) {
				t = rand() % 7; }
			else { t = 7; }
			if (t >= 6) {
				t = rand() % RANDOMIZABLE_TERRAIN_TYPES;
				switch (t) {
					//If you want to add more terrain, you have to do it in two places.
					//Here, and its sister (which has the same comment).
					case 0: spot = MONSTER | UNSEEN_TILE; break;
					case 1: spot = MERCHANT | UNSEEN_TILE; break;
					case 2: spot = WARP | UNSEEN_TILE; break;
					case 3: spot = TRAP | UNSEEN_TILE; break;
					case 4: spot = LOOT | UNSEEN_TILE; break;
					case 5: spot = STATUE | UNSEEN_TILE; break;
					case 6: spot = HEAL | UNSEEN_TILE; break;
				}
				dungeon[i][j][dLv] = spot;
			}
		}
	}
	return true;
}

#endif

