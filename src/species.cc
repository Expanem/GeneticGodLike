/*!
  \file   species.cc
  \date   June 2020
  \brief  species module
*/

#include "species.h"

using namespace std;

void Specie::newTick() {
    this->consuming();
}

void Specie::consuming() {
    this->food_stored -= this->base_food_consumption;
    this->water_stored -= this->base_water_consumption;
    if ((this->water_stored <= 0) or (this->food_stored <= 0)){
        this->dead = true;
    }
}

void Specie::drinking(float water_quantity) {
    if (this->water_stored == this->water_storage) {
        return;
    } else if (this->water_stored <= this->water_storage - water_quantity) {
        this->water_stored = this->water_storage;
        return;
    } else {
        this->water_stored += water_quantity;
    }
}

void Specie::eating(float food_quantity) {
    if (this->food_stored == this->food_storage) {
        return;
    } else if (this->food_stored <= this->food_storage - food_quantity) {
        this->food_stored = this->food_storage;
        return;
    } else {
        this->food_stored += food_quantity;
    }
}