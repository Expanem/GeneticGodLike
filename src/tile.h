#include <iostream>
#include <vector>
#include "species.h"

#ifndef TILE_H
#define TILE_H

enum TYPE {aquatic,fertile,barren};

class Tile{
public :     
    Tile();
    TYPE get_type(){return type;}
private :
    std::vector<Specie*> occupation;
    //std::vector<Tree*> vegetation;
    TYPE type;
    //const unsigned int posX;
    //const unsigned int posY;
    double irrigated; // 0 (far from aquatic tile) to 1 (border with an aquatic tile)
    double height;
    double sunlight;
    double toxicity;
    unsigned int iron_ore;
    unsigned int charcoal;
    unsigned int oil;
};

#endif