#include <iostream>
#include <vector>
#include "species.h"

#ifndef TILE_H
#define TILE_H

enum TYPE {aquatic,fertile,barren,NOTYPE};

class Tile{
public :     
    Tile();
    TYPE get_type(){return type;}
    double get_height(){return height;}
    Specie* get_top();
    bool is_occupied(){return occupied;}
    void add_specie(Specie* specie);
    void remove_specie(Specie* specie);
    void set_height(double new_height);
    void set_type(int new_type);
    void set_pos(unsigned int x, unsigned int y);
    void generate_intra();
private :
    std::vector<Specie*> occupation;
    //std::vector<Tree*> vegetation;
    TYPE type;
    unsigned int posX;
    unsigned int posY;
    double irrigated; // 0 (far from aquatic tile) to 1 (border with an aquatic tile)
    double height;
    double sunlight;
    double toxicity;
    unsigned int iron_ore;
    unsigned int charcoal;
    unsigned int oil;
    bool occupied;
};

#endif