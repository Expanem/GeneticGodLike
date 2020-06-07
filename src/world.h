#include <iostream>
#include <vector>
#include "tile.h"

#ifndef WORLD_H
#define WORLD_H

class World{
public :     
    World();
    ~World();
    void generate();
    const void show();
private :
    std::vector<std::vector<Tile>> environement;
    std::vector<std::vector<Specie*>> population;
};

#endif