/*!
  \file   simulation.cc
  \date   June 2020
  \brief  simulation module
*/

#include <chrono>
#include <thread>
#include <fstream>

#include "simulation.h"
#include "world.h"
#include "const.h"

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

static World* my_world;

/**
 * Create an object of the World and call the rendering functions.
 * 
 * @return 0 if no error.
 */
int initialisation() {
    my_world = new World;
    my_world->show();
    return 0;
}


/**
 * Loop simulation functions to update the world each tick.
 * 
 * @param nb_ticks Number of iterations until the end of the simulated World.
 */
void main_loop(int nb_ticks) {
    for (int i = 0; i < nb_ticks; i++) {
        // cout << i << endl;
        my_world->update_population();
        my_world->update_tiles();
        my_world->show();
        sleep_for(nanoseconds(500000000));
    }
}

/**
 * Save game data in a file.
 * 
 * @param file_name Name or directory+name to save the game.
 */
int save(string file_name) {
    if (file_name == ""){cout << "ERROR wrong save file name" << endl; exit(1);} 
    ofstream flux(file_name, std::ofstream::out);
    vector<Specie*> population = my_world->get_population();
    vector<std::vector<Tile>> environement = my_world->get_environement(); // Should use pointers

    flux << GAME_VERSION << endl
         << world_size << endl;

    for (int x = 0; x < world_size; x++) {
        for (int y = 0; y < world_size; y++) {
            if (environement[x][y].get_type() == fertile) {
                flux << environement[x][y].get_height() << " "
                     << environement[x][y].get_type() << " " << environement[x][y].get_plant()->get_icon() << " " << environement[x][y].get_plant()->get_state() << " " << environement[x][y].get_plant()->is_poisonous() << " " << environement[x][y].get_plant()->get_max_energy()
                     << endl;
            } else {
                flux << environement[x][y].get_height() << " " 
                     << environement[x][y].get_type()
                     << endl;
            }
        }
    }

    flux << population.size() <<endl;

    for (int entity = 0; entity < population.size(); entity++) {
        population[entity]->save(flux);
    }

    flux.close();
}