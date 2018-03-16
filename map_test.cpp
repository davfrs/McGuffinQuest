#include <thread>
#include <chrono>

#include "Graphics.h"
#include "Terrain.h"

using namespace std;

int main()
{
    Graphics graphics;
    for (int i = 0; i < 5; i++)
    {
        graphics.show(VIEW_MAP);
        this_thread::sleep_for(chrono::seconds(1));
    }

}