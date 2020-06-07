/*!
  \file   main.cc
  \date   June 2020
  \brief  main module
*/

#include <iostream>
#include <string>
#include "world.h"

using namespace std;

int main(int argc, char *argv[]) {
    World my_world;
    my_world.generate();
    my_world.show();
}