#include <iostream>
#include <vector>

#ifndef TILE_H
#define TILE_H

enum TYPE {aquatic,fertile,barren}

class Tile{
public :     
    Tile{};

private :
    std::vector<Species*> occupation;
    std::vector<Tree*> vegetation;
    TYPE type;
    const unsigned int posX;
    const unsigned int posY;
    double irrigated; // 0 (far from aquatic tile) to 1 (border with an aquatic tile)
    double height;
    double sunlight;
    double toxicity;
    unsigned int iron_ore;
    unsigned int charcoal;
    unsigned int oil;
};

#endif