//dungeonbytes
//a map that correlates characters / bytes to the terrains

#ifndef DUNGEONBYTES
	#define DUNGEONBYTES
		#define UNSEEN_TILE 0x80
		#define GO_DEEPER '>'
		#define GOALPOINT '='
		#define GO_HIGHER '<'
		#define FLOOR '.'
		//terrain_types needs to equal number of terrains below
		//#define WALL '#'
		#define MONSTER '!'
		#define MERCHANT '%'
		#define WARP '*'
		#define TRAP '^'
		#define LOOT '$'
		#define STATUE '|'
		#define HEAL '+'
		//If you want to add more terrain, you have to do it in two places.
		//Here, and its sister (which has the same comment).
		#define RANDOMIZABLE_TERRAIN_TYPES 7 //non-floor types
		#define USED_STATUE '/'
	
#endif