#include <iostream>
#include <vector>
#include "world.h"
#include "tile.h"
#include "const.h"
    
using namespace std;

World::World()
{
    environement.resize(world_size,vector<Tile>(world_size));
    generate();
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
                cout<<"  ";
                break;
            }
        }
        cout << endl;
    }
    
}

void World::generate()
{
    srand((unsigned)time(0));
    for (int i = 0; i < environement.size(); i++){
        for (int j = 0; j < environement[i].size(); j++){
            int a = rand()%3;
            environement[i][j].set_type(a);
        }
    }
    
    for (size_t i = 0; i < 16; i++)
    {
        smooth_terrain();
    }
    

}

void World::smooth_terrain()
{
    for (unsigned int i = 1; i < environement.size()-1; i++)
    {
        for (unsigned int j = 1; j < environement[i].size()-1; j++)
        {
            unsigned int terraincount [3] = {};
            terraincount[environement[i+1][j].get_type()]++;
            terraincount[environement[i-1][j].get_type()]++;
            terraincount[environement[i][j+1].get_type()]++;
            terraincount[environement[i][j-1].get_type()]++;

            for (size_t m = 0; m < 3; m++)
            {
                if(terraincount[m] > 2) environement[i][j].set_type(m);
            }
            
        }
    }
    
}