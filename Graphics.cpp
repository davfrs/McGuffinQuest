#include "Graphics.h"

void Graphics::redrawMap()
{
    int z = map.playerLocation().Z();

    std::cout << std::string(50, '\n');

    for (int y = 0; y <= MAP_Y_SIZE; y++)
    {
        std::cout << ']';
        for (int x = 0; x < MAP_X_SIZE; x++)
        {
            if (y == 0 || y == MAP_Y_SIZE)
                std::cout << "=";
            else
                std::cout << static_cast<char>(map.revealSquare(x, y, z));

        }
        std::cout << '[' << std::endl;
    }
}

void Graphics::show(VIEW view)
{
    switch(view) {
        case VIEW_MAP:
            redrawMap();
            break;
        case VIEW_INVENTORY:
            // TODO: this
            break;
    }
}
