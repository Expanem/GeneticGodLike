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
    Basics basic_infos_1 = {"Racoon", 'R', 50, 20, 20, 20, 1, 1, 20, 20, 20, 200, 0.5, 10, 2};
    Thresholds threshold_infos_1 = {0.25, 0.25, 0.75, 0.75};
    Coordinates position_infos_1 = {25,25};
    population.push_back(new Fighter_specie(basic_infos_1, position_infos_1, threshold_infos_1));
    environement[25][25].add_specie(population[0]);
}

World::~World()
{

}

const void World::show()
{
    system("clear");
    for (auto latitude : environement)
    {
        for (auto tile : latitude)
        {
            string x = "  ";
            if(tile.is_occupied()) x[1] = tile.get_top()->get_icone();
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
        Coordinates nearest_food = get_nearest_food(population[i]);
        Coordinates nearest_water = get_nearest_water(population[i]);
        int distance_nearest_food = distance(population[i]->get_coordinates(), nearest_food);
        int distance_nearest_water = distance(population[i]->get_coordinates(), nearest_water);
        int action = population[i]->choose_action(distance_nearest_food, distance_nearest_water);
        Coordinates old;
        nearest_water = {30,30};
        nearest_food  = {30,20};
        switch (action)
        {
        case 1: 
            std::cout << nearest_food.x << nearest_food.y << std::endl;
            old = population[i]->get_coordinates();
            population[i]->newTick(action,nearest_food);
            environement[old.x][old.y].remove_specie(population[i]);
            environement[population[i]->get_coordinates().x][population[i]->get_coordinates().y].add_specie(population[i]);
            break;

        case 2: 
            std::cout << nearest_food.x << nearest_food.y << std::endl;
            old = population[i]->get_coordinates();
            population[i]->newTick(action,nearest_water);
            environement[old.x][old.y].remove_specie(population[i]);
            environement[population[i]->get_coordinates().x][population[i]->get_coordinates().y].add_specie(population[i]);
            break;
        
        default:
            break;
        }
    }
}

Coordinates World::get_nearest_food(Specie* specie) {
    Coordinates coord;
    Coordinates ent_pos = specie->get_coordinates();
    for (unsigned int i = 0; i < world_size/2; i++)
    {
        for (int j = -i; j <= i; j++)
        {
            int m = coord.x + j ;
            int n = coord.y + j ;
            if(m >= world_size) m = world_size-1;
            if(n >= world_size) n = world_size-1;
            if(m < 0) m = 0;
            if(n < 0) n = 0;
            if(environement[coord.x+i][n].get_type()==fertile)
            {
                coord.x = coord.x+i;
                coord.y = n;
                return coord;
            }
            if(environement[coord.x-i][n].get_type()==fertile)
            {
                coord.x = coord.x-i;
                coord.y = n;
                return coord;
            }
            if(environement[m][coord.y+i].get_type()==fertile)
            {
                coord.x = m;
                coord.y = coord.y + i;
                return coord;
            }
            if(environement[m][coord.y-i].get_type()==fertile)
            {
                coord.x = m;
                coord.y = coord.y -i;
                return coord;
            }
        }
        return coord;
    }
    return coord;
}

Coordinates World::get_nearest_water(Specie* specie) {
    Coordinates coord;
    Coordinates ent_pos = specie->get_coordinates();
    for (unsigned int i = 0; i < world_size/2; i++)
    {
        for (int j = -i; j <= i; j++)
        {
            int m = coord.x + j ;
            int n = coord.y + j ;
            if(m >= world_size) m = world_size-1;
            if(n >= world_size) n = world_size-1;
            if(m < 0) m = 0;
            if(n < 0) n = 0;
            if(environement[coord.x+i][n].get_type()==aquatic)
            {
                coord.x = coord.x+i;
                coord.y = n;
                return coord;
            }
            if(environement[coord.x-i][n].get_type()==aquatic)
            {
                coord.x = coord.x-i;
                coord.y = n;
                return coord;
            }
            if(environement[m][coord.y+i].get_type()==aquatic)
            {
                coord.x = m;
                coord.y = coord.y + i;
                return coord;
            }
            if(environement[m][coord.y-i].get_type()==aquatic)
            {
                coord.x = m;
                coord.y = coord.y -i;
                return coord;
            }
        }
        return coord;
    }
    return coord;
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