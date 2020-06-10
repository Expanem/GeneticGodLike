#include <iostream>
#include <vector>
#include "world.h"
#include "tile.h"
#include "const.h"
#include <string>
#include "species.h"
#include "tools.h"
    
using namespace std;

World::World()
{
    environement.resize(world_size,vector<Tile>(world_size));
    generate();

    Basics basic_infos_1 = {"Racoon", 'R', 50, 20, 20, 20, 1, 1, 1, 1, 60, 60, 0.5, 10, 2};
    Thresholds threshold_infos_1 = {0.25, 0.25, 0.75, 0.75};
    Coordinates position_infos_1 = {25,25};
    population.push_back(new Fighter_specie(basic_infos_1, position_infos_1, threshold_infos_1));
    environement[25][25].add_specie(population[0]);
    position_infos_1 = {15,15};
    population.push_back(new Fighter_specie(basic_infos_1, position_infos_1, threshold_infos_1));
    environement[15][15].add_specie(population[1]);
}

World::~World()
{

}

const void World::show()
{
    //system("clear");
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

void World::update_population() {
    for (int i = 0; i < population.size(); i++){
        population_interact_with_environement(population[i]);

        Coordinates old_position = population[i]->get_coordinates();
        Coordinates nearest_mate = get_nearest_same_specie(population[i]);
        Coordinates nearest_food = get_nearest_food(population[i]);
        Coordinates nearest_water = get_nearest_water(population[i]);
        int distance_nearest_food = distance(old_position, nearest_food);
        int distance_nearest_water = distance(old_position, nearest_water);
        bool is_alone = false;
        if (population.size() == 1 ) { is_alone = true; }

        if (nearest_mate.x >= 0 and nearest_mate.x < world_size and nearest_mate.y >= 0 and nearest_mate.y < world_size) {
            population_reproduction(population[i], environement[nearest_mate.x][nearest_mate.y].get_top());
        }

        population[i]->update(nearest_food, nearest_water, nearest_mate, is_alone);
        environement[old_position.x][old_position.y].remove_specie(population[i]);
        Coordinates new_position = population[i]->get_coordinates();
        environement[new_position.x][new_position.y].add_specie(population[i]);
    
        if (population[i]->get_state()) {
            environement[new_position.x][new_position.y].remove_specie(population[i]); 
            population[i] = population.back();
            population.pop_back();
        } // Should let the corpse
    }
}

void World::population_interact_with_environement(Specie* entity) {
    Coordinates entity_coordinates = entity->get_coordinates();
    switch (environement[entity_coordinates.x][entity_coordinates.y].get_type())
        {
        case aquatic:
            entity->drink(30);
            break;
        case fertile:
            if (environement[entity_coordinates.x][entity_coordinates.y].get_plant()->is_eatable()) {
                entity->eat(environement[entity_coordinates.x][entity_coordinates.y].get_plant());
            }
            break;
        case barren:
            break;
        default:
            exit(1);
            break;
        }
}

void World::population_reproduction(Specie* entity, Specie* nearest_mate) {
    if(distance(nearest_mate->get_coordinates(), entity->get_coordinates()) == 1){
            entity->increase_reproduced();
            nearest_mate->increase_reproduced();
            if (entity->get_reproduced() >=0 and entity->get_reproduced() < TIME_AFTER_REPRODUCTION) {
            } else if (nearest_mate->get_reproduced() >=0 and nearest_mate->get_reproduced() < TIME_AFTER_REPRODUCTION) {
            } else if ((entity->is_chill()) && (nearest_mate->is_chill())) {
                Genetic_full_data full_infos = entity->reproduction(nearest_mate);
                population.push_back(new Fighter_specie(full_infos.basic_infos, entity->get_coordinates(), full_infos.threshold_infos));
                environement[population.back()->get_coordinates().x][population.back()->get_coordinates().y].add_specie(population.back());
            }
        }
}

void World::debug(int iteration, Coordinates old_position, Coordinates new_position, Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate, int food_stored, int water_stored) {
    std::cout << "----------------------" << std::endl;
    std::cout << "Update of : " << iteration << std::endl;
    std::cout << "OLD POSITION : " << old_position.x << " " << old_position.y << std::endl;
    std::cout << "NEW POSITION : " << new_position.x << " " << new_position.y << std::endl;
    std::cout << "NEAREST MATE : " << nearest_mate.x << " " << nearest_mate.y << std::endl;
    std::cout << "FOOD COORD : " << nearest_food.x << " " << nearest_food.y << std::endl;
    std::cout << "WATER COORD : " << nearest_water.x << " " << nearest_water.y << std::endl;
    std::cout << "FOOD STORED : " << food_stored << std::endl;
    std::cout << "WATER STORED : " << water_stored << std::endl;

}

Coordinates World::get_nearest_food(Specie* specie) {
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

Coordinates World::get_nearest_same_specie(Specie* specie) {
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
                    return mate_coord;
                }
            }
        }
    }
} 

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