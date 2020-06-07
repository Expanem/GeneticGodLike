#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include "tile.h"
#include "tools.h"

class World{
public :     
    World();
    ~World();
    const void show();
    //void numeric();
    void update_population();
private :
    void generate();
    void smooth_terrain(unsigned int times, unsigned int res);
    void smooth_height(unsigned int times);
    Coordinates get_nearest_food(int specie_ID);
    Coordinates get_nearest_water(int specie_ID);

    std::vector<std::vector<Tile>> environement;
    std::vector<Specie*> population;
};

#endif