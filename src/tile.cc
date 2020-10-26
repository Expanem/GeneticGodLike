#include <iostream>
#include "tile.h"
#include "const.h"
#include "vegetation.h"
#include "species.h"

/**
 * Tile Constructor.
 */
Tile::Tile()
{
    type = aquatic;
}

/**
 * Type setter.
 * 
 * @param new_type Desired new type.
 */
void Tile::set_type(int new_type)
{
    if(new_type < 0 || new_type > 2) type = NOTYPE;
    else type = (TYPE)new_type;
    if(type == fertile) plant = new Vegetation(PLANT_ENERGY,0,false,20,0.1,0.2,growing,':');
}

/**
 * Position setter.
 * 
 * @param x Desired new x coordinate.
 * @param y Desired new y coordinate.
 */
void Tile::set_pos(unsigned int x, unsigned int y)
{
    posX = x;
    posY = y;
}

/**
 * Generate internal data for the tile, given the type.
 */
void Tile::generate_intra()
{
    switch (type)
    {
    case aquatic:
        height = 0;
        irrigated = 1;
        sunlight = 1;
        iron_ore = 0;
        charcoal = 0;
        oil = 0;
        break;
    
    case fertile:
        irrigated = 1;
        sunlight = 1;
        height = rand()%(int)max_height;
        iron_ore = 0;
        charcoal = rand()%max_capacity;
        oil = rand()%max_capacity;
        break;
    
    case barren:
        sunlight = 1;
        height = rand()%(int)max_height;
        irrigated = 0;
        iron_ore = rand()%max_capacity;
        charcoal = rand()%max_capacity;
        oil = rand()%max_capacity;
        break;
    
    default:
        break;
    }
}

/**
 * Height setter.
 * 
 * @param new_height Desired new height.
 */
void Tile::set_height(double new_height)
{
    if(new_height < 0) height = 0;
    else if(new_height > max_height) height = max_height;
    else height = new_height;
}

/**
 * Add specie to the tile.
 * 
 * @param specie Pointer to the desired specie.
 */
void Tile::add_specie(Specie* specie)
{
    occupation.push_back(specie);
    occupied = true;
}

/**
 * Remove specie from the tile.
 * 
 * @param specie Pointer to the desired specie.
 */
void Tile::remove_specie(Specie* specie)
{
    bool found = false;
    for (int i = 0; i < occupation.size(); i++)
    {
        if(specie == occupation[i])
        {
            found = true;
            occupation[i] = occupation.back();
            occupation.pop_back();

            if(occupation.empty()) occupied = false;
        }
    }
    if(not found) std::cout<<specie->get_name()<<" could not be found."<<std::endl;
}

/**
 * Get the top entity on the tile.
 * 
 * @return Pointer to the entity at the top of the tile.
 */
Specie* Tile::get_top()
{
    if(occupation.empty()) return nullptr;
    else return occupation[0];
}

/**
 * Update the tile.
 */
void Tile::update()
{
    plant->update(1,1);
}

/**
 * Check if there is a corpse on the tile.
 * 
 * @return Pointer to a corpse if there is one.
 */
Specie* Tile::get_corpse() {
    for (int i = 0; i < occupation.size(); i++) {
        if(occupation[i]->get_state() == dead) {
            return occupation[i];
        }
    }
    return nullptr;
}

/**
 * Check if the tile as an obstacle.
 * 
 * @param x x coordinate of the desired tile. 
 * @param y y coordinate of the desired tile. 
 * 
 * @return True if there is an obstable on the tile.
 */
bool Obstacles_map::is_obstacle(int x, int y){
    // TO DO
    return false;
}

/**
 * Check if the tile as an obstacle.
 * 
 * @param coord Coordinates of the desired tile. 
 * 
 * @return True if there is an obstable on the tile.
 */
bool Obstacles_map::is_obstacle(Coordinates coord){
    // TO DO
    return false;
}