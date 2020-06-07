/*!
  \file   species.cc
  \date   June 2020
  \brief  species module
*/

#include "species.h"

using namespace std;

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