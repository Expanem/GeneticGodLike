/*!
  \file   tools.h
  \date   June 2020
  \brief  tools header
*/

#ifndef TOOLS_H
#define TOOLS_H

typedef struct Coordinates {
    int x;
    int y;
} Coordinates;

int distance(Coordinates a, Coordinates b);

#endif