//dungeonbytes
//a map that correlates characters / bytes to the terrains
#ifndef DUNGEONBYTES
#define DUNGEONBYTES
/*
class TerrainTileBase {
    const char displayChar;
    unsigned char index;
public:
    inline char getDisplayChar() { return this->displayChar; }
    inline void __setIndex(unsigned char index) { this->index = index; }
    inline unsigned char getIndex() { return this->index; }
    virtual bool attemptMovingInto() { return true; }//return if the movement failed, like trying to walk into a walk
    virtual void useTile() = 0;//open loot chest, interact with a merchant, etc.
};
class TileRegistry {
    TerrainTileBase* allTiles[128];
    unsigned char usedUpTo;
public:
    TileRegistry() :usedUpTo(0) {}
    ~TileRegistry() {
        for(int i=0;i<usedUpTo;i++)
            delete this->allTiles[i];
    }
    void addTile(TerrainTileBase* tile) {
        tile->__setIndex(this->usedUpTo);
        this->allTiles[this->usedUpTo++] = tile;
    }
    bool isExposedTile(unsigned char index) {
        return (index & 0x80) == 0x00;
    }
    unsigned char revealTile(unsigned char index) {
        return index | 0x80;
    }
    unsigned char unrevealTile(unsigned char index) {
        return index & ~0x80;
    }
    bool isValidTile(unsigned char index) {
        return revealTile(index) < this->usedUpTo;
    }
    TerrainTileBase* getExposedTile(unsigned char index) {
        return this->allTiles[index];
    }
};
*/
const int RANDOMIZABLE_TERRAIN_TYPES = 7; //non-floor types
const char UNSEEN_TILE_CHARACTER = ' ';
const char PLAYER_TILE_CHARACTER = 'X';
enum DUNGEONBYTES
{
    UNSEEN_TILE = 0x80,
    GO_DEEPER = '>',
    GOALPOINT = '=',
    GO_HIGHER = '<',
    FLOOR = '.',
//terrain_types needs to equal number of terrains below
// WALL = '#',
    MONSTER = '!',
    MERCHANT = '%',
    WARP = '*',
    TRAP = '^',
    LOOT = '$',
    STATUE = '|',
    HEAL = '+',
//If you want to add more terrain, you have to do it in two places.
//Here, and its sister (which has the same comment).
    USED_STATUE = '/'
};

#endif