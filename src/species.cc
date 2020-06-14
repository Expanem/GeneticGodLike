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
    type(basic_infos.type),
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

    food_stored(40),
    water_stored(40),
    deviation(0),
    tick_lived(0),
    state(alive),
    reproduced(-1),

    coord(position_info),
    objective({0,0}),

    velocity_storage(0),
    
    threshold_urgent_food(threshold_infos.threshold_urgent_food),
    threshold_urgent_water(threshold_infos.threshold_urgent_water),
    threshold_chill_food(threshold_infos.threshold_chill_food),
    threshold_chill_water(threshold_infos.threshold_chill_water) {
}

void Specie::update(Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey, Coordinates nearest_prey_corpse, Coordinates nearest_predator, bool is_alone) {
    std::cout << "UPDATING" << std::endl;
    int distance_nearest_food = distance(coord, nearest_food);
    int distance_nearest_water = distance(coord, nearest_water);
    int distance_nearest_prey, distance_nearest_prey_corpse, distance_nearest_predator;
    if (nearest_prey.x != -1 and nearest_prey.y != -1) { distance_nearest_prey = distance(coord, nearest_prey); } else { distance_nearest_prey = world_size + 1; }
    if (nearest_prey_corpse.x != -1 and nearest_prey_corpse.y != -1) { distance_nearest_prey_corpse = distance(coord, nearest_prey_corpse); } else { distance_nearest_prey_corpse = world_size + 1; }
    if (nearest_predator.x != -1 and nearest_predator.y != -1) { distance_nearest_predator = distance(coord, nearest_predator); } else { distance_nearest_predator = world_size + 1; }

    // IF CARN OR OMNI ? WHICH ARGUMENTS ?
    ACTION action = choose_action(distance_nearest_food, distance_nearest_water, distance_nearest_predator);
    std::cout << "OBJECTIVE " << action << std::endl;
    if (is_alone && action == to_mate) {action = do_nothing;}
    this->consume(1.0);
    switch (action) {
    case to_eat:
        switch (diet) {
        case herbivore:
            objective = nearest_food;
            break;
        case carnivore:
            if ((nearest_prey_corpse.x != -1 and nearest_prey_corpse.y != -1)) { // CHOOSE BETTER MAX DIST
                std::cout << "going to nearest corpse at : " <<  nearest_prey_corpse.x << " " << nearest_prey_corpse.y << std::endl;
                objective = nearest_prey_corpse;
            } if (nearest_prey.x != -1 and nearest_prey.y != -1) {
                std::cout << "going to nearest prey at : " <<  nearest_prey.x << " " << nearest_prey.y << std::endl;
                objective = nearest_prey;
            } else {
                std::cout << "NO FOOD FOUND" << std::endl;
            }
            
            break;
        case omnivore:
            objective = nearest_food; // For now
            break;
        default:
            break;
        }
        move_to_objective();
        break; // WATER
    case to_drink:
        objective = nearest_water;
        move_to_objective();
        break;
    case to_mate: // MATE
        if (nearest_mate.x >= 0 and nearest_mate.x < world_size and nearest_mate.y >= 0 and nearest_mate.y < world_size) {
            objective = nearest_mate; // MUST BE BEFORE THE OBJECTIVE
            move_to_objective(1);
        } else { }
        break;
    case to_flee: // FLEE / HIDE
        if (nearest_predator.x != -1 and nearest_predator.y != -1) {
            objective = nearest_predator;
            move_away_from_objective();
        } 
        break;
    default:
        break;
    }
}

void Specie::consume(float ratio) {
    this->food_stored -= this->food_consumption;
    this->water_stored -= this->water_consumption;
    if ((this->water_stored <= 0) or (this->food_stored <= 0)){
        state = dead;
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
    if (water_stored > water_storage) {
        water_stored = water_storage;
    }
}

void Specie::eat(Vegetation* plant) {
    if (food_stored == food_storage) { return; }  
    else {
        if (plant->is_poisonous()){ state = dead; }
        food_stored += plant->eat();
        if (food_stored > food_storage) { food_stored = food_storage; }
    }
}

void Specie::eat(Specie* prey) {
    if (food_stored == food_storage) { return; }  
    else {
        food_stored += prey->be_eaten();
        if (food_stored > food_storage) { food_stored = food_storage; }
    }
}

double Specie::be_eaten() { 
    state = disapeared;
    return food_stored;
}

void Specie::move_to_objective(int distance_max) { //Keep velocity energy without taking care of direction
    velocity_storage += velocity;
    while ((velocity + velocity_storage) > 1) {
        this->consume(RATIO_VELOCITY_FOOD_CONSUMPTION);
        velocity_storage -= 1;
        int distance_x = objective.x - coord.x;
        int distance_y = objective.y - coord.y;
        if (abs(distance_x) + abs(distance_y) == distance_max){ return; }
        else if (abs(distance_x) + abs(distance_y) < distance_max) { 
            if (abs(distance_x) > abs(distance_y)) {
                if (distance_x > 0) {
                    coord.x -= 1;
                } else {
                coord.x += 1;
                }
            } else {
                if (distance_y > 0) {
                    coord.y -= 1;
                } else {
                    coord.y += 1;
                }
            }
         } else {
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
}

void Specie::move_away_from_objective(int distance_max) { // MERGE WITH MOVE_TO_OBJ  !
    velocity_storage += velocity;
    while ((velocity + velocity_storage) > 1) {
        this->consume(RATIO_VELOCITY_FOOD_CONSUMPTION);
        velocity_storage -= 1;
        int distance_x = objective.x - coord.x;
        int distance_y = objective.y - coord.y;
        if ((abs(distance_x) + abs(distance_y)) >= distance_max) { return; }
        else {
            if (abs(distance_x) < abs(distance_y)) {
                if (distance_x > 0) {
                    coord.x -= 1;
                } else {
                coord.x += 1;
                }
            } else {
                if (distance_y > 0) {
                    coord.y -= 1;
                } else {
                    coord.y += 1;
                }
            }
        }
    }
}

Genetic_full_data Specie::reproduction(Specie* mate) {
    this->reset_reproduced();
    mate->reset_reproduced();
    this->consume(10.0);
    mate->consume(10.0);
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

ACTION Specie::choose_action(int distance_nearest_food, int distance_nearest_water, int distance_nearest_predator) {
    float food_per = food_stored / food_storage;
    float water_per = water_stored / water_storage;
    cout << distance_nearest_predator << endl;
    if (abs(distance_nearest_predator) <= 2 ) { // CHOOSE BETTER NUMBER
        return to_flee;
    }
    if (food_per <= 0 or water_per <= threshold_urgent_water) {
        if (food_per <= water_per) {
            return to_eat;
        } else {
            return to_drink;
        }
    } else if (is_chill() == false) {
        if (food_per <= water_per) {
            return to_eat;
        } else {
            return to_drink;
        }
    } else if (is_chill()) {
            return to_mate;
    }
}

bool Specie::is_chill() {
    float food_per = food_stored / food_storage;
    float water_per = water_stored / water_storage;
    if (food_per < threshold_chill_food or water_per < threshold_chill_water) {
        return false;
    } else { 
        return true;
    }
}

void Specie::fight(Specie* entity) {
    consume(FIGHT_FOOD_CONSUMPTION);
    if (attack >= entity->get_defense()) {
        entity->set_state(dead);
    } else {
        entity->consume(entity->get_defense() - attack);
    }
}

/*
Pacifist_specie::Pacifist_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos) 
   :  Specie(basic_infos, position_info, threshold_infos) {

}

void Pacifist_specie::fight(Specie* entity) {
    consume(FIGHT_FOOD_CONSUMPTION);
    if (attack > entity->get_defense()) {
        entity->consume(attack - entity->get_defense());
    } else {
    }
}

Fighter_specie::Fighter_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos)
   :  Specie(basic_infos, position_info, threshold_infos) {
}

void Fighter_specie::fight(Specie* entity) {
    consume(FIGHT_FOOD_CONSUMPTION);
    if (attack >= entity->get_defense()) {
        entity->set_dead();
    } else {
        entity->consume(entity->get_defense() - attack);
    }
}
*/