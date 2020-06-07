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

const void World::show()
{
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            cout<<tile.get_type();
            switch (tile.get_type())
            {
            case aquatic:
                cout << "\033[36m\u2588\u2588\033[0m";
                break;

            case fertile:
                cout << "\033[32m\u2588\u2588\033[0m";
                break;

            case barren:
                cout << "\033[1,30m\u2588\u2588\033[0m";
                break;
            
            default:
                break;
            }
        }
        cout << endl;
    }
    
}

void World::generate()
{
    srand((unsigned)time(0));
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            int a = rand()%3;
            cout<<a<<", ";
            tile.set_type(a);
        }
    }
    cout<<"Done generating terrain."<<endl;
}