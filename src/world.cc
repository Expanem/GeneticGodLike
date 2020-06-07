#include <iostream>
#include <vector>
#include "world.h"
#include "tile.h"
#include "const.h"
    
using namespace std;

World::World()
{
    environement.resize(world_size,vector<Tile>(world_size));
}

World::~World()
{

}

void World::show()
{
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            switch (tile.get_type())
            {
            case aquatic:
                cout << "\033[36m\u25A0\033[0m";
                break;

            case fertile:
                cout << "\033[32m\u25A0\033[0m";
                break;

            case barren:
                cout << "\033[1,30m\u25A0\033[0m";
                break;
            
            default:
                break;
            }
        }
        cout << endl;
    }
    
}