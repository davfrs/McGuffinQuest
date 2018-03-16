#include "Terrain.h"

//Returns false if generation failed.
bool Map::generateLevel(int depth)
{
    if (depth > MAP_Z_SIZE)
        return false;

    COORDINATE2 wayUp{0, 0};
    COORDINATE2 wayDown{0, 0};

    while (wayUp.XCo == wayDown.XCo && wayUp.YCo == wayDown.YCo)
    {
        wayUp.XCo = rand() % MAP_X_SIZE;
        wayDown.XCo = rand() % MAP_X_SIZE;

        wayUp.YCo = rand() % MAP_Y_SIZE;
        wayDown.YCo = rand() % MAP_Y_SIZE;
    }

    bool connect = false;
    if (depth != MAP_Z_SIZE)
        dungeon[wayDown.XCo][wayDown.YCo][depth] = GO_DEEPER | UNSEEN_TILE;
    else
        dungeon[wayDown.XCo][wayDown.YCo][depth] = GOALPOINT | UNSEEN_TILE;

    dungeon[wayUp.XCo][wayUp.YCo][depth] = GO_HIGHER;
    int cX;
    int cY;
    cX = wayUp.XCo;
    cY = wayUp.YCo;
    while (!connect)
    {
        if (cX == wayDown.XCo && cY == wayDown.YCo)
            connect = true; //we made it~
        else
        {//(cX != wayUp.XCo || cY != wayUp.YCo)
            //we didn't make it yet, so set old point to floor-type and move a space.
            dungeon[cX][cY][depth] = FLOOR;
            switch (rand() % 4)
            {
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
#ifdef WRAPPING_LEVELS
            if (cX == -1)
                cX = MAP_X_SIZE - 1;

            if (cY == -1)
                cY = MAP_Y_SIZE - 1;

            if (cX == MAP_X_SIZE)
                cX = 0;

            if (cY == MAP_Y_SIZE)
                cY = 0;
#else
            if (cX == -1)
                cX = 0;

            if (cY == -1)
                cY = 0;

            if (cX == MAP_X_SIZE)
                cX = MAP_X_SIZE - 1;

            if (cY == MAP_Y_SIZE)
                cY = MAP_Y_SIZE - 1;
#endif
        } //end else
    } //end while !connect
    //ie: we now have a connection between wayDown and wayUp!
    char spot = FLOOR;
    for (int i = 0; i < MAP_X_SIZE; i++)
    {
        for (int j = 0; j < MAP_Y_SIZE; j++)
        {
            int t = 0;
            if (dungeon[i][j][depth] == FLOOR)
                t = rand() % 7;
            else
                t = 7;

            if (t >= 6)
            {
                switch (rand() % RANDOMIZABLE_TERRAIN_TYPES)
                {
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
                dungeon[i][j][depth] = spot;
            }
        }
    }
}