//dungeonbytes
//a map that correlates characters / bytes to the terrains
#ifndef DUNGEONBYTES
#define DUNGEONBYTES

const int RANDOMIZABLE_TERRAIN_TYPES = 7 //non-floor types

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