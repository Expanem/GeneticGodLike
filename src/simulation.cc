/*!
  \file   simulation.cc
  \date   June 2020
  \brief  simulation module
*/

#include "simulation.h"
#include "world.h"

using namespace std;

static World* my_world;

int initialisation() {
    my_world = new World;
    my_world->show();
    return 0;
}

void main_loop(int nb_ticks) {
    for (int i = 0; i < nb_ticks; i++) {
        cout << i << endl;
        my_world->update_population();
    }
    my_world->show();
}