#include <iostream>
#include <vector>
#include "tile.h"

#ifndef WORLD_H
#define WORLD_H

class World{
public :     
    World();
    ~World();
    const void show();
    //void numeric();
private :
    void generate();
    void smooth_terrain();

    std::vector<std::vector<Tile>> environement;
    std::vector<std::vector<Specie*>> population;
};

#endif