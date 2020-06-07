#include <iostream>
#include "tile.h"
#include "const.h"

Tile::Tile()
{
    type = aquatic;
}

void Tile::set_type(int new_type)
{
    if(new_type < 0 || new_type > 2) type = NOTYPE;
    else type = (TYPE)new_type;
}

void Tile::set_pos(unsigned int x, unsigned int y)
{
    posX = x;
    posY = y;
}

void Tile::generate_intra()
{
    switch (type)
    {
    case aquatic:
        height = 0;
        irrigated = 1;
        iron_ore = 0;
        charcoal = 0;
        oil = 0;
        break;
    
    case fertile:
        height = rand()%(int)max_height;
        iron_ore = 0;
        charcoal = rand()%max_capacity;
        oil = rand()%max_capacity;
        break;
    
    case barren:
        height = rand()%(int)max_height;
        irrigated = 0;
        iron_ore = rand()%max_capacity;
        charcoal = rand()%max_capacity;
        oil = rand()%max_capacity;
        break;
    
    default:
        break;
    }
}

void Tile::set_height(double new_height)
{
    if(new_height < 0) height = 0;
    else if(new_height > max_height) height = max_height;
    else height = new_height;
}