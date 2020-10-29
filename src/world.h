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

    std::vector<Entity*> get_population() {return population;};
    std::vector<std::vector<Tile>> get_environement() {return environement;};
private :
    void generate();
    void smooth_terrain(unsigned int times, unsigned int res);
    void smooth_height(unsigned int times);

    void population_interact_with_environement(Entity* entity);
    void population_interact_with_population(Entity* entity, Entity* nearest_non_mate);
    void population_reproduction(Entity* entity, Entity* nearest_mate);
    STATE population_update_state(int iteration);

    void debug(int iteration, Coordinates old_position, Coordinates new_position, Coordinates nearest_vege_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey_coord, Coordinates nearest_prey_corpse_coord, int food_stored, int water_stored);

    Coordinates get_nearest_vege_food(Entity* entity);
    Coordinates get_nearest_water(Entity* entity);
    Entity* get_nearest_same_entity(Entity* entity);
    Entity* get_nearest_other_entity(Entity* entity);
    Entity* get_nearest_prey(Entity* entity, STATE state = alive);
    Entity* get_nearest_predator(Entity* entity);

    std::vector<std::vector<Tile>> environement;
    std::vector<Entity*> population;
    Obstacles_map obstacles;
};

#endif