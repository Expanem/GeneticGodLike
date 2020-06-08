/*!
  \file   species.cc
  \date   June 2020
  \brief  species module
*/

#include "species.h"
#include "const.h"
#include "tile.h"
#include <iostream>

using namespace std;

Specie::Specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos)   
  : name(basic_infos.name),
    icon(basic_infos.icon),
    size(basic_infos.size),
    weight(basic_infos.weight),
    strengh(basic_infos.strengh),
    attack(basic_infos.attack),
    defense(basic_infos.defense),
    velocity(basic_infos.velocity),
    food_consumption(basic_infos.food_consumption),
    water_consumption(basic_infos.water_consumption),
    water_storage(basic_infos.water_storage),
    food_storage(basic_infos.food_storage),
    libido(basic_infos.libido),
    life_span(basic_infos.life_span),
    diet(basic_infos.diet),

    food_stored(20),
    water_stored(10),
    deviation(0),
    tick_lived(0),
    dead(false),

    coord(position_info),
    objective({0,0}),

    velocity_storage(0),
    
    threshold_urgent_food(threshold_infos.threshold_urgent_food),
    threshold_urgent_water(threshold_infos.threshold_urgent_water),
    threshold_chill_food(threshold_infos.threshold_chill_food),
    threshold_chill_water(threshold_infos.threshold_chill_water) {
}

void Specie::update(Tile* actual_tile, Coordinates nearest_food, Coordinates nearest_water) {
    int distance_nearest_food = distance(coord, nearest_food);
    int distance_nearest_water = distance(coord, nearest_water);
    int action = choose_action(distance_nearest_food, distance_nearest_water);
    switch (actual_tile->get_type())
    {
    case aquatic:
        drink(1); // REALLY THIS MUCH !!!!!!!!!!!!?????
        break;
    case fertile:
        eat(actual_tile->get_plant());
        break;
    case barren:
        break;
    default:
        std::cout << "ERROR" << std::endl;
        exit(1);
        break;
    }
    this->consume(1.0);
    switch (action) {
    case 1: // FOOD
        objective = nearest_food;
        move_to_objective();
        break; // WATER
    case 2:
        objective = nearest_water;
        move_to_objective();
        break;
    case 3: // MATE
        move_to_objective();
        break;
    default:
        break;
    }
}

void Specie::consume(float ratio) {
    this->food_stored -= this->food_consumption;
    this->water_stored -= this->water_consumption;
    if ((this->water_stored <= 0) or (this->food_stored <= 0)){
        this->dead = true;
    }
}

void Specie::drink(float water_quantity) {
    if (this->water_stored == this->water_storage) {
        return;
    } else if (this->water_stored <= this->water_storage - water_quantity) {
        this->water_stored = this->water_storage;
        return;
    } else {
        this->water_stored += water_quantity;
    }
}

void Specie::eat(Vegetation* plant) {
    // TO DO !!!!!!!!!!!!
    int food_quantity = 0;
    if (this->food_stored == this->food_storage) {
        return;
    } else if (this->food_stored <= this->food_storage - food_quantity) {
        this->food_stored = this->food_storage;
        return;
    } else {
        this->food_stored += food_quantity;
    }
}

void Specie::move_to_objective() { //Keep velocity energy without taking care of direction
    velocity_storage += velocity;
    while ((velocity + velocity_storage) > 1) {
        this->consume(RATIO_VELOCITY_FOOD_CONSUMPTION);
        velocity_storage -= 1;
        int distance_x = objective.x - coord.x;
        int distance_y = objective.y - coord.y;
        if (distance_x == 0 and distance_y == 0){
            return;
        }
        if (abs(distance_x) > abs(distance_y)) {
            if (distance_x > 0) {
                coord.x += 1;
            } else {
                coord.x -= 1;
            }
        } else {
            if (distance_y > 0) {
                coord.y += 1;
            } else {
                coord.y -= 1;
            }
        }
    }
}

int Specie::choose_action(float distance_nearest_food, float distance_nearest_water) {
    float food_per = food_stored / food_storage;
    float water_per = water_stored / water_storage;
    if (food_per <= threshold_urgent_food or water_per <= threshold_urgent_water) {
        if (food_per <= water_per) {
            return 1; // Choose food
        } else {
            return 2; // Choose water
        }
    } else if (food_per < threshold_chill_food or water_per < threshold_chill_water) {
        if (food_per <= water_per) {
            return 1; // Choose food
        } else {
            return 2; // Choose water
        }
    } else {
            return 3; // Try to mate
    }
}

Pacifist_specie::Pacifist_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos) 
   :  Specie(basic_infos, position_info, threshold_infos) {

}

Fighter_specie::Fighter_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos)
   :  Specie(basic_infos, position_info, threshold_infos) {
}

Basics basic_infos_1 = {"Racoon", 'R', 50, 20, 20, 20, 20, 1, 20, 20, 20, 20, 0.5, 10, 2};
Thresholds threshold_infos_1 = {0.25, 0.25, 0.75, 0.75};
Coordinates position_infos_1 = {0,0};
static Fighter_specie *Racoon = new Fighter_specie(basic_infos_1, position_infos_1, threshold_infos_1);