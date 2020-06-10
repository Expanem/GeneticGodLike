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
    void update_tiles();
private :
    void generate();
    void smooth_terrain(unsigned int times, unsigned int res);
    void smooth_height(unsigned int times);
    Coordinates get_nearest_food(Specie* specie);
    Coordinates get_nearest_water(Specie* specie);
        Coordinates get_nearest_same_specie(Specie* specie);
    Coordinates can_reproduce_with(Specie* entity);

    std::vector<std::vector<Tile>> environement;
    std::vector<Specie*> population;
};

#endif