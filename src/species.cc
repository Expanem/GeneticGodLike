/*!
  \file   species.cc
  \date   June 2020
  \brief  species module
*/

#include "species.h"
#include "const.h"
#include <iostream>
#include <random>

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
    reproduced(-1),

    coord(position_info),
    objective({0,0}),

    velocity_storage(0),
    
    threshold_urgent_food(threshold_infos.threshold_urgent_food),
    threshold_urgent_water(threshold_infos.threshold_urgent_water),
    threshold_chill_food(threshold_infos.threshold_chill_food),
    threshold_chill_water(threshold_infos.threshold_chill_water) {
}

void Specie::update(Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate) {
    int distance_nearest_food = distance(coord, nearest_food);
    int distance_nearest_water = distance(coord, nearest_water);
    int action = choose_action(distance_nearest_food, distance_nearest_water);
    std::cout << "OBJECTIVE " << action << std::endl;
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
        objective = nearest_mate; // MUST BE BEFORE THE OBJECTIVE
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
    if (food_stored == food_storage) { std::cout << "MAXED FOOD1" << std::endl; return; }  
    else {
        if (plant->is_poisonous()){ dead = true; }
        cout << "FOOD BEFORE " << food_stored << std::endl;
        food_stored += plant->eat();
        cout << "FOOD AFTER " << food_stored << std::endl;
        if (food_stored > food_storage) { std::cout << "MAXED FOOD2" << std::endl; food_stored = food_storage;}
    }
}

void Specie::move_to_objective() { //Keep velocity energy without taking care of direction
    std::cout << "THIS IS MY OBJECTIVE " << objective.x << " " << objective.y << std::endl; 
    velocity_storage += velocity;
    std::cout << "VELOCITY STORED" << velocity_storage << std::endl;
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

Genetic_full_data Specie::reproduction(Specie* mate) {
    this->reset_reproduced();
    mate->reset_reproduced();
    this->consume(5.0);
    mate->consume(5.0);
    if (deviation < 0.5) {
        srand(NULL);
        Basics basic_infos;
        if (rand()%1 < 0.5) { basic_infos.name=this->name; } else { basic_infos.name=mate->name; }
        if (rand()%1 < 0.5) { basic_infos.icon=this->icon; } else { basic_infos.icon=mate->icon; }
        if (rand()%1 < 0.5) { basic_infos.size=this->size; } else { basic_infos.size=mate->size; }
        if (rand()%1 < 0.5) { basic_infos.weight=this->weight; } else { basic_infos.weight=mate->weight; }
        if (rand()%1 < 0.5) { basic_infos.strengh=this->strengh; } else { basic_infos.strengh=mate->strengh; }
        if (rand()%1 < 0.5) { basic_infos.attack=this->attack; } else { basic_infos.attack=mate->attack; }
        if (rand()%1 < 0.5) { basic_infos.defense=this->defense; } else { basic_infos.defense=mate->defense; }
        if (rand()%1 < 0.5) { basic_infos.velocity=this->velocity; } else { basic_infos.velocity=mate->velocity; }
        if (rand()%1 < 0.5) { basic_infos.food_consumption=this->food_consumption; } else { basic_infos.food_consumption=mate->food_consumption; }
        if (rand()%1 < 0.5) { basic_infos.water_consumption=this->water_consumption; } else { basic_infos.water_consumption=mate->water_consumption; }
        if (rand()%1 < 0.5) { basic_infos.water_storage=this->water_storage; } else { basic_infos.water_storage=mate->water_storage; }
        if (rand()%1 < 0.5) { basic_infos.food_storage=this->food_storage; } else { basic_infos.food_storage=mate->food_storage; }
        if (rand()%1 < 0.5) { basic_infos.libido=this->libido; } else { basic_infos.libido=mate->libido; }
        if (rand()%1 < 0.5) { basic_infos.life_span=this->life_span; } else { basic_infos.life_span=mate->life_span; }
        if (rand()%1 < 0.5) { basic_infos.diet=this->diet; } else { basic_infos.diet=mate->diet; }
        Threshold threshold_infos;
        if (rand()%1 < 0.5) { threshold_infos.threshold_urgent_food=this->threshold_urgent_food; } else { threshold_infos.threshold_urgent_food=mate->threshold_urgent_food; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_urgent_water=this->threshold_urgent_water; } else { threshold_infos.threshold_urgent_water=mate->threshold_urgent_water; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_chill_food=this->threshold_chill_food; } else { threshold_infos.threshold_chill_food=mate->threshold_chill_food; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_chill_water=this->threshold_chill_water; } else { threshold_infos.threshold_chill_water=mate->threshold_chill_water; }
        Genetic_full_data genetics = {basic_infos,threshold_infos};
        return genetics; 
    }
}

int Specie::choose_action(float distance_nearest_food, float distance_nearest_water) {
    float food_per = food_stored / food_storage;
    float water_per = water_stored / water_storage;
    if (food_per <= 0 or water_per <= threshold_urgent_water) {
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