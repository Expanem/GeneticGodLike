#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include "tile.h"
#include "tools.h"

class World{
public :     
    World();
    ~World();
    const void show();
    //void numeric();
    void update_population();
    void update_tiles();

    std::vector<Specie*> get_population() {return population;};
    std::vector<std::vector<Tile>> get_environement() {return environement;};
private :
    void generate();
    void smooth_terrain(unsigned int times, unsigned int res);
    void smooth_height(unsigned int times);

    void population_interact_with_environement(Specie* entity);
    void population_interact_with_population(Specie* entity, Specie* nearest_non_mate);
    void population_reproduction(Specie* entity, Specie* nearest_mate);
    STATE population_update_state(int iteration);

    void debug(int iteration, Coordinates old_position, Coordinates new_position, Coordinates nearest_vege_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey_coord, Coordinates nearest_prey_corpse_coord, int food_stored, int water_stored);

    Coordinates get_nearest_vege_food(Specie* specie);
    Coordinates get_nearest_water(Specie* specie);
    Specie* get_nearest_same_specie(Specie* specie);
    Specie* get_nearest_other_specie(Specie* specie);
    Specie* get_nearest_prey(Specie* specie, STATE state = alive);
    Specie* get_nearest_predator(Specie* specie);

    std::vector<std::vector<Tile>> environement;
    std::vector<Specie*> population;
    Obstacles_map obstacles;
};

#endif