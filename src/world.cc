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
        Coordinates current = population[i]->get_coordinates();
        std::cout << "----------------------" << std::endl;
        std::cout << "Update of : " << i << std::endl;
        std::cout << "OLD POSITION " << current.x << " " << current.y << std::endl;
        switch (environement[current.x][current.y].get_type())
        {
        case aquatic:
            population[i]->drink(40); // REALLY THIS MUCH !!!!!!!!!!!!?????
            break;
        case fertile:
            cout << "EATING" << endl;
            population[i]->eat(environement[current.x][current.y].get_plant());
            break;
        case barren:
            break;
        default:
            std::cout << "ERROR" << std::endl;
            exit(1);
            break;
        }
        cout << "T" << endl;
        Coordinates nearest_mate = can_reproduce_with(population[i]);
        cout << "NEAREST MATE " << nearest_mate.x << " " << nearest_mate.y << endl;
        cout << "TT" << endl;
        if ((nearest_mate.x != -1) && (nearest_mate.y != -1)) {
            cout << "I'M GONNA MATE !!!" << endl;
            Genetic_full_data full_infos = population[i]->reproduction(environement[nearest_mate.x][nearest_mate.y].get_top());
            population.push_back(new Fighter_specie(full_infos.basic_infos, population[i]->get_coordinates(), full_infos.threshold_infos));
            environement[population.back()->get_coordinates().x][population.back()->get_coordinates().y].add_specie(population.back());
        }
        cout << "TTT" << endl;
        Coordinates nearest_food = get_nearest_food(population[i]);
        Coordinates nearest_water = get_nearest_water(population[i]);
        int distance_nearest_food = distance(current, nearest_food);
        int distance_nearest_water = distance(current, nearest_water);
        
        std::cout << "FOOD COORD " << nearest_food.x << " " << nearest_food.y << std::endl;
        std::cout << "WATER COORD " << nearest_water.x << " " << nearest_water.y << std::endl;
        population[i]->update(nearest_food, nearest_water, nearest_mate);
        environement[current.x][current.y].remove_specie(population[i]);
        environement[population[i]->get_coordinates().x][population[i]->get_coordinates().y].add_specie(population[i]);
        current = population[i]->get_coordinates();
        std::cout << "NEW POSITION " << population[i]->get_coordinates().x << " " << population[i]->get_coordinates().y << std::endl;
        std::cout << "FOOD STORED " << population[i]->get_food_stored() << std::endl;
        std::cout << "WATER STORED " << population[i]->get_water_stored() << std::endl;
    
        if (population[i]->get_state()) {
            environement[current.x][current.y].remove_specie(population[i]); 
            population[i] = population.back();
            population.pop_back();
        } // Should let the corpse
    }
}

Coordinates World::can_reproduce_with(Specie* entity) {
    if (entity->get_reproduced() >=0 and entity->get_reproduced() < TIME_AFTER_REPRODUCTION) {
        entity->increase_reproduced();
        return {-1,-1};
    } else {  
        for (int clock_x = -1; clock_x <= 1; clock_x++){
            for (int clock_y = -1; clock_y <= 1; clock_y++){
                if (clock_x == 0 and clock_y == 0) {}
                else if (environement[entity->get_coordinates().x + clock_x][entity->get_coordinates().y + clock_y].is_occupied() == true ) {  // IS OCCUPIED NOT WORKING
                    return {entity->get_coordinates().x + clock_x, entity->get_coordinates().y + clock_y};
                }
            }
        }
        return {-1,-1};
    }
}

Coordinates World::get_nearest_food(Specie* specie) {
    Coordinates specie_coord = specie->get_coordinates();
    /* 
    if(environement[specie_coord.x][specie_coord.y].get_type()==fertile){
        return specie_coord;
    } */
    Coordinates food_coord = {0,0};
    int half_world_size = world_size / 2;
    for (int radius = 0; radius < half_world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                food_coord.x = specie_coord.x + clock_x;
                food_coord.y = specie_coord.y + clock_y;
                if (food_coord.x < 0 || food_coord.x >= world_size){}
                else if (food_coord.y < 0 || food_coord.y >= world_size){}
                else if (food_coord.x == specie_coord.x && food_coord.y == specie_coord.y){}
                else if(environement[food_coord.x][food_coord.y].get_type()==fertile){
                    cout << "FOUND SOME FOOD MAYBE NOT EATABLE" << food_coord.x << " " << food_coord.y << std::endl;
                    return food_coord;

                    /*
                    if (environement[food_coord.x][food_coord.y].get_plant()->is_eatable() == true) {
                        return food_coord;
                        cout << "FOUND SOME FOOD" << food_coord.x << " " << food_coord.y << std::endl;
                    } */ // NOT WORKING !!!!
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
    int half_world_size = world_size / 2;
    for (int radius = 0; radius < half_world_size; radius++){
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

void World::update_tiles()
{
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            tile.update();
        }
    }
}