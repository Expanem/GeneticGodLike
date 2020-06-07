#include <iostream>
#include "tile.h"

Tile::Tile()
{
    type = barren;
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