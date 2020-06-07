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
    std::cout << type << std::endl;
}