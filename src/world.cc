#include <iostream>
#include <vector>
#include "world.h"
#include "tile.h"
#include "const.h"
#include <string>
#include "species.h"
#include "tools.h"
    
using namespace std;

/**
 * World Constructor.
 */
World::World()
{
    srand(time(NULL));
    
    environement.resize(world_size,vector<Tile>(world_size));
    generate();

    Basics basic_infos_1 = {"Racoon", prey, 'R', 0.3, 10, 1, 1, 1, 0.5, 100, omnivore};
    Thresholds threshold_infos_1 = {0.25, 0.25, 0.75, 0.75};
    Coordinates position_infos_1 = {25,25};
    Genetics genetic_infos_1 = {0.01};
    Learnt learnt_infos_1 = {{"Eagle"}};

    population.push_back(new Specie(basic_infos_1, position_infos_1, threshold_infos_1, genetic_infos_1, learnt_infos_1));
    environement[25][25].add_specie(population.back());
    position_infos_1 = {15,15};
    population.push_back(new Specie(basic_infos_1, position_infos_1, threshold_infos_1, genetic_infos_1, learnt_infos_1));
    environement[15][15].add_specie(population.back());
    position_infos_1 = {17,17};
    population.push_back(new Specie(basic_infos_1, position_infos_1, threshold_infos_1, genetic_infos_1, learnt_infos_1));
    environement[17][17].add_specie(population.back());
    
    learnt_infos_1 = {{}};
    int pos_x = rand() % world_size;
    int pos_y = rand() % world_size;
    basic_infos_1 = {"Eagle", predator, 'E', 0.3, 10, 5, 2, 2, 0.5, 100, carnivore};
    threshold_infos_1 = {0.25, 0.25, 0.9, 0.9};
    position_infos_1 = {pos_x, pos_y};
    population.push_back(new Specie(basic_infos_1, position_infos_1, threshold_infos_1, genetic_infos_1, learnt_infos_1));
    environement[pos_x][pos_y].add_specie(population.back());
    pos_x = rand() % world_size;
    pos_y = rand() % world_size;
    position_infos_1 = {pos_x,pos_y};
    population.push_back(new Specie(basic_infos_1, position_infos_1, threshold_infos_1, genetic_infos_1, learnt_infos_1));
    environement[pos_x][pos_y].add_specie(population.back());

}

/**
 * World destructor.
 */
World::~World()
{

}

/**
 * Show the world on the user's console. 
 */
const void World::show()
{
    // system("clear");
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            string x = "  ";
            if(tile.is_occupied()) x[1] = tile.get_top()->get_icon();
            switch (tile.get_type())
            {
            case aquatic:
                cout << "\033[31;46m"<<x<<"\033[0m";
                break;

            case fertile:
                x[0] = tile.get_plant()->get_icon();
                cout << "\033[31;42m"<<x<<"\033[0m";
                break;

            case barren:
                cout << "\033[31;1;40m"<<x<<"\033[0m";
                break;
            
            default:
                cout<<"  ";
                break;
            }
        }
        cout << endl;
    }
    
}

/**
 * Generate the world map.
 */
void World::generate()
{
    srand((unsigned)time(0));
    for (int i = 1; i < environement.size()-1; i++){
        for (int j = 1; j < environement[i].size()-1; j++){
            int a = rand()%3;
            environement[i][j].set_type(a);
        }
    }
    
    smooth_terrain(3,3);
    
    for (int i = 1; i < environement.size()-1; i++){
        for (int j = 1; j < environement[i].size()-1; j++){
            environement[i][j].generate_intra();
        }
    }

    smooth_height(1);
}

/**
 * Smooth the height of the generated map.
 * 
 * @param times Number of iteration of the algo.
 */
void World::smooth_height(unsigned int times)
{
    for (unsigned int t = 0 ; t < times ; t++)
    {
        for (unsigned int i = 1; i < environement.size()-1; i++)
        {
            for (unsigned int j = 1; j < environement[i].size()-1; j++)
            {
                double mean = 0;
                mean += environement[i+1][j+1].get_height();
                mean += environement[i+1][j].get_height();
                mean += environement[i+1][j-1].get_height();
                mean += environement[i-1][j+1].get_height();
                mean += environement[i-1][j].get_height();
                mean += environement[i-1][j-1].get_height();
                mean += environement[i][j+1].get_height();
                mean += environement[i][j-1].get_height();
                environement[i][j].set_height(mean/8);
            }
        }
    }
}

/**
 * Smooth the terrain composition of the generated map.
 * 
 * @param times Number of iteration of the algo.
 * @param res Resolution of the algo.
 */
void World::smooth_terrain(unsigned int times, unsigned int res)
{
    for (unsigned int t =0 ; t < times ; t++)
    {
        for (unsigned int i = 1; i < environement.size()-1; i++)
        {
            for (unsigned int j = 1; j < environement[i].size()-1; j++)
            {
                unsigned int terraincount [3] = {};
                terraincount[environement[i+1][j].get_type()]++;
                terraincount[environement[i-1][j].get_type()]++;
                terraincount[environement[i][j+1].get_type()]++;
                terraincount[environement[i][j-1].get_type()]++;
                terraincount[environement[i+1][j+1].get_type()]++;
                terraincount[environement[i-1][j-1].get_type()]++;
                terraincount[environement[i+1][j-1].get_type()]++;
                terraincount[environement[i-1][j+1].get_type()]++;

                for (size_t m = 0; m < res; m++)
                {
                    if(terraincount[m] > 3) environement[i][j].set_type(m);
                }
                
            }
        }
    }
}

/**
 * Main loop to update at every tick all the entities.
 */
void World::update_population() {
    for (int i = 0; i < population.size(); i++){
        STATE state = population_update_state(i);
        if ( state == alive) {
            Specie* nearest_non_mate = get_nearest_other_specie(population[i]);

            population_interact_with_environement(population[i]);
            if (nearest_non_mate != nullptr ) { population_interact_with_population(population[i], nearest_non_mate); }

            Coordinates old_position = population[i]->get_coordinates();
            Specie* nearest_mate = get_nearest_same_specie(population[i]);
            Coordinates nearest_mate_coord = {-1,-1};
            if (nearest_mate != nullptr) { nearest_mate_coord = nearest_mate->get_coordinates(); }
            Coordinates nearest_vege_food_coord = get_nearest_vege_food(population[i]);
            Coordinates nearest_water_coord = get_nearest_water(population[i]);
            Specie* nearest_prey = get_nearest_prey(population[i]);
            Coordinates nearest_prey_coord = {-1,-1};
            if (nearest_prey != nullptr) { nearest_prey_coord = nearest_prey->get_coordinates(); }
            Specie* nearest_prey_corpse = get_nearest_prey(population[i], dead);
            Coordinates nearest_prey_corpse_coord = {-1,-1};
            if (nearest_prey_corpse != nullptr) { nearest_prey_corpse_coord = nearest_prey_corpse->get_coordinates(); }
            Specie* nearest_predator = get_nearest_predator(population[i]);
            Coordinates nearest_predator_coord = {-1, -1};
            if (nearest_predator != nullptr) { nearest_predator_coord = nearest_predator->get_coordinates(); }

            int distance_nearest_vege_food = distance(old_position, nearest_vege_food_coord);
            int distance_nearest_water = distance(old_position, nearest_water_coord);
            bool is_alone = false;
            if (population.size() == 1 ) { is_alone = true; } // NEED TO USE IT BETTER

            if (nearest_mate_coord.x >= 0 and nearest_mate_coord.x < world_size and nearest_mate_coord.y >= 0 and nearest_mate_coord.y < world_size) {
                population_reproduction(population[i], nearest_mate);
            }
            population[i]->update(nearest_vege_food_coord, nearest_water_coord, nearest_mate_coord, nearest_prey_coord, nearest_prey_corpse_coord, nearest_predator_coord, is_alone);
            environement[old_position.x][old_position.y].remove_specie(population[i]);
            Coordinates new_position = population[i]->get_coordinates();
            environement[new_position.x][new_position.y].add_specie(population[i]);
            
            // debug(i, old_position, new_position, nearest_vege_food_coord, nearest_water_coord, nearest_mate_coord, nearest_prey_coord, nearest_prey_corpse_coord, population[i]->get_food_stored(), population[i]->get_water_stored());

            state = population_update_state(i);
        }
        if (state == disapeared) {i--;}
    }
}

/**
 * Return the state of an entity.
 * 
 * @param i Entity ID
 * 
 * @return Boolean
 */
STATE World::population_update_state(int i) {
    if (population[i]->get_state() == dead) { 
        population[i]->set_icon('X');
        return dead;
    } else if (population[i]->get_state() == disapeared) {
        environement[population[i]->get_coordinates().x][population[i]->get_coordinates().y].remove_specie(population[i]); 
        population[i] = population.back();
        population.back() = nullptr;
        population.pop_back();
        return disapeared;
    }
    return alive;
}

/**
 * Make an entity intereact with his close environement.
 * 
 * @param entity Pointer to the entity.
 */
void World::population_interact_with_environement(Specie* entity) {
    Coordinates entity_coordinates = entity->get_coordinates();
    switch (environement[entity_coordinates.x][entity_coordinates.y].get_type())
        {
        case aquatic:
            entity->drink(30);
            break;
        case fertile:
            if (entity->get_diet() == omnivore || entity->get_diet() == herbivore) {
                if (environement[entity_coordinates.x][entity_coordinates.y].get_plant()->is_eatable()) {
                    entity->eat(environement[entity_coordinates.x][entity_coordinates.y].get_plant());
            }
            }
            break;
        case barren:
            break;
        default:
            exit(1);
            break;
        }
    if (entity->get_diet() == carnivore) { // To do with omnivore too
        if (environement[entity_coordinates.x][entity_coordinates.y].get_corpse() != nullptr) {
            Specie* corpse = environement[entity_coordinates.x][entity_coordinates.y].get_corpse();
            if (corpse->get_name() != entity->get_name()) {
                entity->eat(corpse);
            }
        }
    }

}

/**
 * Smooth the height of the generated map.
 * 
 * @param entity Pointer to the entity.
 * @param nearest_non_mate Nearest entity of a different specie.
 */
void World::population_interact_with_population(Specie* entity, Specie* nearest_non_mate) {
    Coordinates entity_coordinates = entity->get_coordinates();
    Coordinates nearest_non_mate_coordinates = nearest_non_mate->get_coordinates();
    if (distance(entity_coordinates, nearest_non_mate_coordinates) <= 1) {
        entity->fight(nearest_non_mate);
    }
}

/**
 * Reproduce the population.
 * 
 * @param entity Pointer to the entity.
 * @param nearest_non_mate Nearest entity of the same specie.
 */
void World::population_reproduction(Specie* entity, Specie* nearest_mate) {
    if(distance(nearest_mate->get_coordinates(), entity->get_coordinates()) == 1){
            entity->increase_reproduced();
            nearest_mate->increase_reproduced();
            if (entity->get_reproduced() >=0 and entity->get_reproduced() < TIME_AFTER_REPRODUCTION) {
            } else if (nearest_mate->get_reproduced() >=0 and nearest_mate->get_reproduced() < TIME_AFTER_REPRODUCTION) {
            } else if ((entity->is_chill()) && (nearest_mate->is_chill())) {
                Genetic_full_data full_infos = entity->reproduction(nearest_mate);
                population.push_back(new Specie(full_infos.basic_infos, entity->get_coordinates(), full_infos.threshold_infos, full_infos.genetic_infos, full_infos.learnt_infos));
                environement[population.back()->get_coordinates().x][population.back()->get_coordinates().y].add_specie(population.back());
            }
        }
}


/**
 * Print everything for debug purpose.
 * 
 * @param iteration
 * @param old_position
 * @param new_position
 * @param nearest_vege_food_coord
 * @param nearest_water_coord
 * @param nearest_mate_coord
 * @param nearest_prey_coord
 * @param nearest_prey_corpse_coord
 * @param food_stored
 * @param water_stored
 */
void World::debug(int iteration, Coordinates old_position, Coordinates new_position, Coordinates nearest_vege_food_coord, Coordinates nearest_water_coord, Coordinates nearest_mate_coord, Coordinates nearest_prey_coord, Coordinates nearest_prey_corpse_coord, int food_stored, int water_stored) {
    std::cout << "----------------------" << std::endl;
    std::cout << "Update of : " << iteration << std::endl;
    std::cout << "OLD POSITION : " << old_position.x << " " << old_position.y << std::endl;
    std::cout << "NEW POSITION : " << new_position.x << " " << new_position.y << std::endl;
    std::cout << "NEAREST MATE : " << nearest_mate_coord.x << " " << nearest_mate_coord.y << std::endl;
    std::cout << "FOOD COORD : " << nearest_vege_food_coord.x << " " << nearest_vege_food_coord.y << std::endl;
    std::cout << "WATER COORD : " << nearest_water_coord.x << " " << nearest_water_coord.y << std::endl;
    std::cout << "PREY COORD : " << nearest_prey_coord.x << " " << nearest_prey_coord.y << std::endl;
    std::cout << "CORPSE COORD : " << nearest_prey_corpse_coord.x << " " << nearest_prey_corpse_coord.y << std::endl;
    std::cout << "FOOD STORED : " << food_stored << std::endl;
    std::cout << "WATER STORED : " << water_stored << std::endl;

}

/**
 * Compute nearest tile with vege food.
 * 
 * @param specie Pointer to the entity.
 * 
 * @return Nearest vege food coordinates.
 */
Coordinates World::get_nearest_vege_food(Specie* specie) {
    Coordinates specie_coord = specie->get_coordinates();
    Coordinates food_coord = {-2,-2};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                food_coord.x = specie_coord.x + clock_x;
                food_coord.y = specie_coord.y + clock_y;
                if (food_coord.x < 0 || food_coord.x >= world_size){}
                else if (food_coord.y < 0 || food_coord.y >= world_size){}
                else if (food_coord.x == specie_coord.x && food_coord.y == specie_coord.y){}
                else if(environement[food_coord.x][food_coord.y].get_type()==fertile){
                    if (environement[food_coord.x][food_coord.y].get_plant()->is_eatable()) {
                        return food_coord;
                    } 
                }
            }
        }
    }
} 

/**
 * Compute nearest tile with water.
 * 
 * @param specie Pointer to the entity.
 * 
 * @return Nearest water coordinates.
 */
Coordinates World::get_nearest_water(Specie* specie) {
    Coordinates specie_coord = specie->get_coordinates();
    if(environement[specie_coord.x][specie_coord.y].get_type()==aquatic){ 
        return specie_coord;
    }
    Coordinates water_coord = {0,0};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                water_coord.x = specie_coord.x + clock_x;
                water_coord.y = specie_coord.y + clock_y;
                if (water_coord.x < 0 || water_coord.x >= world_size){}
                else if (water_coord.y < 0 || water_coord.y >= world_size){}
                else if(environement[water_coord.x][water_coord.y].get_type()==aquatic){
                    return water_coord;
                
                }
            }
        }
    }
} 

/**
 * Compute nearest tile with same specie mate.
 * 
 * @param specie Pointer to the entity.
 * 
 * @return Nearest same specie entity coordinates.
 */
Specie* World::get_nearest_same_specie(Specie* specie) {
    Coordinates specie_coord = specie->get_coordinates();
    Coordinates mate_coord = {-1,-1};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                mate_coord.x = specie_coord.x + clock_x;
                mate_coord.y = specie_coord.y + clock_y;
                if (mate_coord.x < 0 || mate_coord.x >= world_size){}
                else if (mate_coord.y < 0 || mate_coord.y >= world_size){}
                else if (mate_coord.x == specie_coord.x && mate_coord.y == specie_coord.y){}
                else if(environement[mate_coord.x][mate_coord.y].is_occupied()){
                    if (environement[mate_coord.x][mate_coord.y].get_top()->get_name() == specie->get_name()) { // WHAT IF NON TOP ??
                        return environement[mate_coord.x][mate_coord.y].get_top();
                    }
                }
            }
        }
    }
    return nullptr;
} 

/**
 * Compute nearest tile with other specie entity.
 * 
 * @param specie Pointer to the entity.
 * 
 * @return Nearest other specie entity coordinates.
 */
Specie* World::get_nearest_other_specie(Specie* specie) { // USE IT TO SIMPLIFY THE OTHERS, DONT NEED THIS 4 TIMES XD
    Coordinates specie_coord = specie->get_coordinates();
    Coordinates other_coord = {-1,-1};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                other_coord.x = specie_coord.x + clock_x;
                other_coord.y = specie_coord.y + clock_y;
                if (other_coord.x < 0 || other_coord.x >= world_size){}
                else if (other_coord.y < 0 || other_coord.y >= world_size){}
                else if (other_coord.x == specie_coord.x && other_coord.y == specie_coord.y){}
                else if(environement[other_coord.x][other_coord.y].is_occupied()){
                    if (environement[other_coord.x][other_coord.y].get_top()->get_name() != specie->get_name()) {  // WHAT IF NON TOP
                        return environement[other_coord.x][other_coord.y].get_top();
                    }
                }
            }
        }
    }
    return nullptr;
} 

/**
 * Compute nearest tile with prey.
 * 
 * @param specie Pointer to the entity.
 * @param state State of the entity looked for.
 * 
 * @return Nearest prey coordinates.
 */
Specie* World::get_nearest_prey(Specie* specie, STATE state) { // NEED TO CORRECT IT, GETTING NEAREST OTHER SPECIE FOR NOW
    Coordinates specie_coord = specie->get_coordinates();
    Coordinates prey_coord = {-1,-1};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                prey_coord.x = specie_coord.x + clock_x;
                prey_coord.y = specie_coord.y + clock_y;
                if (prey_coord.x < 0 || prey_coord.x >= world_size){}
                else if (prey_coord.y < 0 || prey_coord.y >= world_size){}
                else if (prey_coord.x == specie_coord.x && prey_coord.y == specie_coord.y){}
                else if(environement[prey_coord.x][prey_coord.y].is_occupied()){
                    if (environement[prey_coord.x][prey_coord.y].get_top()->get_name() != specie->get_name()) { // WHAT IF NON TOP
                        if (environement[prey_coord.x][prey_coord.y].get_top()->get_state() == state) { // MAKE IT CLEAN !
                            if (environement[prey_coord.x][prey_coord.y].get_top()->get_type() == prey) {
                                return environement[prey_coord.x][prey_coord.y].get_top();
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
} 

/**
 * Compute nearest tile with predator.
 * 
 * @param specie Pointer to the entity.
 * 
 * @return Nearest predator coordinates.
 */
Specie* World::get_nearest_predator(Specie* specie) { // NEED TO CORRECT IT, GETTING NEAREST OTHER SPECIE FOR NOW
    Coordinates specie_coord = specie->get_coordinates();
    Coordinates predator_coord = {-1,-1};
    for (int radius = 0; radius < world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                predator_coord.x = specie_coord.x + clock_x;
                predator_coord.y = specie_coord.y + clock_y;
                if (predator_coord.x < 0 || predator_coord.x >= world_size){}
                else if (predator_coord.y < 0 || predator_coord.y >= world_size){}
                else if (predator_coord.x == specie_coord.x && predator_coord.y == specie_coord.y){}
                else if(environement[predator_coord.x][predator_coord.y].is_occupied()){
                    if (specie->is_predator(environement[predator_coord.x][predator_coord.y].get_top()->get_name())) { // WHAT IF NON TOP
                        if (environement[predator_coord.x][predator_coord.y].get_top()->get_type() == predator) {
                            return environement[predator_coord.x][predator_coord.y].get_top();
                        }
                    }
                }
            }
        }
    }
    return nullptr;
} 

/**
 * Update tiles every tick.
 */
void World::update_tiles()
{
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            if(tile.get_type() == fertile)  tile.update();
        }
    }
}