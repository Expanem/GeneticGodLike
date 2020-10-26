/*!
  \file   tools.cc
  \date   June 2020
  \brief  tools module
*/
#include "math.h"
#include "tools.h"

/**
 * Compute distance between two coordinates.
 * 
 * @param a First coordinate.
 * @param b Second coordinate.
 * 
 * @return Distance between a and b.
 */
int distance(Coordinates a, Coordinates b){
    return (abs(a.x - b.x) + abs(a.y - b.y));
}