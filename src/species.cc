/*!
  \file   species.cc
  \date   June 2020
  \brief  species module
*/

#include "species.h"

using namespace std;

Specie::Specie(Basics basic_infos, Thresholds threshold_infos)   
  : name(basic_infos.name),
    size(basic_infos.size),
    weight(basic_infos.weight),
    strengh(basic_infos.strengh),
    attack(basic_infos.attack),
    defense(basic_infos.defense),
    velocity(basic_infos.velocity),
    base_food_consumption(basic_infos.base_food_consumption),
    base_water_consumption(basic_infos.base_water_consumption),
    food_consumption(basic_infos.food_consumption),
    water_consumption(basic_infos.water_consumption),
    water_storage(basic_infos.water_storage),
    food_storage(basic_infos.food_storage),
    water_stored(basic_infos.water_stored),
    food_stored(basic_infos.food_stored),
    deviation(basic_infos.deviation),
    libido(basic_infos.libido),
    life_span(basic_infos.life_span),
    diet(basic_infos.diet),
    tick_lived(0),
    dead(false) {
}

void Specie::newTick() {
    this->consume();
}

void Specie::consume() {
    this->food_stored -= this->base_food_consumption;
    this->water_stored -= this->base_water_consumption;
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

void Specie::eat(float food_quantity) {
    if (this->food_stored == this->food_storage) {
        return;
    } else if (this->food_stored <= this->food_storage - food_quantity) {
        this->food_stored = this->food_storage;
        return;
    } else {
        this->food_stored += food_quantity;
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

Pacifist_specie::Pacifist_specie(Basics basic_infos, Thresholds threshold_infos) 
   :  Specie(basic_infos, threshold_infos) {


}

Fighter_specie::Fighter_specie(Basics basic_infos, Thresholds threshold_infos)
   :  Specie(basic_infos, threshold_infos) {

}

Basics basic_infos_1 = {"Racoon", 50, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 0.001, 0.5, 10, 2};
Thresholds threshold_infos_1 = {0.25, 0.25, 0.75, 0.75};
static Fighter_specie *Racoon = new Fighter_specie(basic_infos_1, threshold_infos_1);