#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <vector>
#include "entities.h"
#include "vegetation.h"

enum TYPE {aquatic,fertile,barren,NOTYPE};

class Obstacles_map {
    public :
        bool is_obstacle(int x, int y); // To do
        bool is_obstacle(Coordinates coord); // To do
    private :

};

class Tile{
public :     
    Tile();
    TYPE get_type(){return type;}
    double get_height(){return height;}
    Entity* get_top();
    Vegetation* get_plant(){return plant;}
    Entity* get_corpse();
    bool is_occupied(){return occupied;}
    void add_entity(Entity* entity);
    void remove_entity(Entity* entity);
    void set_height(double new_height);
    void set_type(int new_type);
    void set_pos(unsigned int x, unsigned int y);
    void generate_intra();
    void update();
private :
    std::vector<Entity*> occupation;
    Vegetation* plant;
    TYPE type;
    unsigned int posX;
    unsigned int posY;
    double irrigated; // 0 (far from aquatic tile) to 1 (border with an aquatic tile)
    double height;
    double sunlight;
    double toxicity;
    unsigned int iron_ore;
    unsigned int charcoal;
    unsigned int oil;
    bool occupied;
};

#endif