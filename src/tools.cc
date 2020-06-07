/*!
  \file   tools.cc
  \date   June 2020
  \brief  tools module
*/
#include "math.h"
#include "tools.h"

int distance(Coordinates a, Coordinates b){
    return (abs(a.x - b.x) + abs(a.y - b.y));
}