#include <iostream>
#include "tile.h"

Tile::Tile()
{
    type = TYPE(rand()%3);
}
