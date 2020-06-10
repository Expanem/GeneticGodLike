/*!
  \file   main.cc
  \date   June 2020
  \brief  main module
*/

#include <iostream>
#include <string>
#include "simulation.h"

using namespace std;

int main(int argc, char *argv[]) {
    initialisation();
    main_loop(1000);
}