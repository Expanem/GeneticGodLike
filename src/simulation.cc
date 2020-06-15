/*!
  \file   simulation.cc
  \date   June 2020
  \brief  simulation module
*/

#include <chrono>
#include <thread>

#include "simulation.h"
#include "world.h"

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

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
        my_world->update_tiles();
        my_world->show();
        sleep_for(nanoseconds(500000000));
    }
}