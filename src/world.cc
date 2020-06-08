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
        Coordinates current = population[i]->get_coordinates();
        int distance_nearest_food = distance(current, nearest_food);
        int distance_nearest_water = distance(current, nearest_water);
        
        population[i]->update(environement[current.x][current.y],nearest_food,nearest_water)
    }
}

Coordinates World::get_nearest_food(Specie* specie) {
    Coordinates specie_coord = specie->get_coordinates();
    if(environement[specie_coord.x][specie_coord.y].get_type()==fertile){
        return specie_coord;
    }
    Coordinates food_coord = {0,0};
    int half_world_size = world_size / 2;
    for (int radius = 0; radius < half_world_size; radius++){
        for (int clock_x = -radius; clock_x <= radius; clock_x++){
            for (int clock_y = -radius; clock_y <= radius; clock_y++){
                food_coord.x = specie_coord.x + clock_x;
                food_coord.y = specie_coord.y + clock_y;
                if (food_coord.x < 0 || food_coord.x >= world_size){}
                else if (food_coord.y < 0 || food_coord.y >= world_size){}
                else if(environement[food_coord.x][food_coord.y].get_type()==fertile){
                    std::cout << "FIND " << food_coord.x << " " << food_coord.y << std::endl;
                    return food_coord;
                
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
                    std::cout << "FIND " << water_coord.x << " " << water_coord.y << std::endl;
                    return water_coord;
                
                }
            }
        }
    }
} 

/*
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
*/
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