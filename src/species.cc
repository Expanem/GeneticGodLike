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

/**
 * Specie Constructor.
 * 
 * @param basic_infos
 * @param position_info
 * @param threshold_infos
 * @param genetic_infos
 * @param learnt_infos
 */
Specie::Specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos, Genetics genetic_infos, Learnt learnt_infos)   
  : name(basic_infos.name),
    type(basic_infos.type),
    icon(basic_infos.icon),
    size(basic_infos.size),
    strength(basic_infos.strength),
    attack(basic_infos.attack),
    defense(basic_infos.defense),
    velocity(basic_infos.velocity),
    libido(basic_infos.libido),
    life_span(basic_infos.life_span),
    diet(basic_infos.diet),

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
    threshold_chill_water(threshold_infos.threshold_chill_water),
    
    mutation_factor(genetic_infos.mutation_factor),
    
    predators(learnt_infos.predators) {
        characteristics_update();
        food_stored = INITIAL_STARTING_FOOD * food_storage;
        water_stored = INITIAL_STARTING_WATER * water_storage;

        std::cout << name << " " << icon << " AT " << coord.x << "," << coord.y << " FOOD STORAGE " << food_storage << " CONSUME " << water_consumption << " WATER STORAGE " << water_storage << " CONSUME " << base_food_consumption << " MASS " << mass << std::endl;

}

/**
 * Compute internal data for the entity.
 */
void Specie::characteristics_update() {
    mass =  RATIO_SIZE_STRENGTH_MASS * size * strength + RATIO_ATTACK_MASS * attack + RATIO_DEFENSE_MASS * defense;
    base_food_consumption = RATIO_MASS_FOOD_CONSUMPTION * mass;
    water_consumption = RATIO_FOOD_WATER_CONSUMPTION * base_food_consumption;
    food_storage = RATIO_MASS_SIZE_FOOD_STORAGE * mass * size;
    water_storage = RATIO_FOOD_WATER_STORAGE * food_storage;
    view_distance = (int) RATIO_SIZE_VIEW_DISTANCE * size;
}

/**
 * Update each tick the entity.
 * 
 * @param nearest_food
 * @param nearest_water
 * @param nearest_mate
 * @param nearest_prey
 * @param nearest_prey_corpse
 * @param nearest_predator
 * @param is_alone
 */
void Specie::update(Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey, Coordinates nearest_prey_corpse, Coordinates nearest_predator, bool is_alone) {
    int distance_nearest_food = distance(coord, nearest_food);
    int distance_nearest_water = distance(coord, nearest_water);
    int distance_nearest_prey, distance_nearest_prey_corpse, distance_nearest_predator;
    if (nearest_prey.x != -1 and nearest_prey.y != -1) { distance_nearest_prey = distance(coord, nearest_prey); } else { distance_nearest_prey = world_size + 1; }
    if (nearest_prey_corpse.x != -1 and nearest_prey_corpse.y != -1) { distance_nearest_prey_corpse = distance(coord, nearest_prey_corpse); } else { distance_nearest_prey_corpse = world_size + 1; }
    if (nearest_predator.x != -1 and nearest_predator.y != -1) { distance_nearest_predator = distance(coord, nearest_predator); } else { distance_nearest_predator = world_size + 1; }

    // IF CARN OR OMNI ? WHICH ARGUMENTS ?
    ACTION action = choose_action(distance_nearest_food, distance_nearest_water, distance_nearest_predator);
    // std::cout << "OBJECTIVE " << action << std::endl;
    if (is_alone && action == to_mate) {action = do_nothing;}

    this->consume(1.0);
    aging();

    switch (action) {
    case to_eat:
        switch (diet) {
        case herbivore:
            objective = nearest_food;
            break;
        case carnivore:
            if ((nearest_prey_corpse.x != -1 and nearest_prey_corpse.y != -1)) { // CHOOSE BETTER MAX DIST
                // std::cout << "going to nearest corpse at : " <<  nearest_prey_corpse.x << " " << nearest_prey_corpse.y << std::endl;
                objective = nearest_prey_corpse;
            } else if (nearest_prey.x != -1 and nearest_prey.y != -1) {
                // std::cout << "going to nearest prey at : " <<  nearest_prey.x << " " << nearest_prey.y << std::endl;
                objective = nearest_prey;
            } else {
                // std::cout << "NO FOOD FOUND" << std::endl;
            }
            break;
        case omnivore:
            objective = nearest_food; // For now
            break;
        default:
            break;
        }
        move_to_objective();
        break;
    case to_drink:
        objective = nearest_water;
        move_to_objective();
        break;
    case to_mate:
        if (nearest_mate.x >= 0 and nearest_mate.x < world_size and nearest_mate.y >= 0 and nearest_mate.y < world_size) {
            objective = nearest_mate; // MUST BE BEFORE THE OBJECTIVE
            move_to_objective(1);
        } else { }
        break;
    case to_flee:
        if (nearest_predator.x != -1 and nearest_predator.y != -1) {
            objective = nearest_predator;
            move_away_from_objective(view_distance);
        } 
        break;
    default:
        break;
    }
}

/**
 * Age the entity and make it die.
 */
void Specie::aging() {
    tick_lived++;
    if (tick_lived >= life_span) {
        state = dead;
    }
}

/**
 * Make the entity consume food and waterm given his data and the ratio.
 * 
 * @param ratio Multiply the consumption by this ratio, given the action.
 */
void Specie::consume(double ratio) {
    this->food_stored -= this->base_food_consumption;
    this->water_stored -= this->water_consumption;
    if ((this->water_stored <= 0) or (this->food_stored <= 0)){
        state = dead;
    }
}

/**
 * Make the entity drink at a water source.
 * 
 * @param water_quantity How much the source can give in one tick.
 */
void Specie::drink(double water_quantity) {
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

/**
 * Make the entity eat a plant.
 * 
 * @param plant Pointer to the plant.
 */
void Specie::eat(Vegetation* plant) {
    if (food_stored == food_storage) { return; }  
    else {
        if (plant->is_poisonous()){ state = dead; }
        food_stored += plant->eat();
        if (food_stored > food_storage) { food_stored = food_storage; }
    }
}

/**
 * Make the entity eat a dead prey.
 * 
 * @param prey Pointer to the prey.
 */
void Specie::eat(Specie* prey) {
    if (food_stored == food_storage) { return; }  
    else {
        food_stored += prey->be_eaten();
        if (food_stored > food_storage) { food_stored = food_storage; }
    }
}

/**
 * Delete the entity after being eaten.
 * 
 * @return Food stored by the entity.
 */
double Specie::be_eaten() { 
    state = disapeared;
    return food_stored;
}

/**
 * Move the entity to his objective.
 * 
 * @param distance_max Maximum distance the entity tries to approach.
 */
void Specie::move_to_objective(int distance_max) { //Keep velocity energy without taking care of direction
    velocity_storage += velocity;
    while ((velocity + velocity_storage) > 1) {
        this->consume(VELOCITY_FOOD_CONSUMPTION * velocity * velocity);
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

/**
 * Flee from his objective.
 * 
 * @param distance_max Distance to escape from.
 */
void Specie::move_away_from_objective(int distance_max) { // MERGE WITH MOVE_TO_OBJ  !
    velocity_storage += velocity;
    while ((velocity + velocity_storage) > 1) {
        this->consume(VELOCITY_FOOD_CONSUMPTION * velocity * velocity);
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

/**
 * Reproducte the entity with a mate.
 * 
 * @param mate Pointer to an other entity of the same Specie.
 * 
 * @return Genetic information : basic_infos + threshold_infos + genetic_infos + learnt_infos.
 */
Genetic_full_data Specie::reproduction(Specie* mate) {
    this->reset_reproduced();
    mate->reset_reproduced();
    this->consume(REPRODUCTION_FOOD_CONSUMPTION);
    mate->consume(REPRODUCTION_FOOD_CONSUMPTION);
    if (deviation < 0.5) {
        srand(NULL);
        Basics basic_infos;
        if (rand()%1 < 0.5) { basic_infos.name=this->name; } else { basic_infos.name=mate->name; }
        if (rand()%1 < 0.5) { basic_infos.icon=this->icon; } else { basic_infos.icon=mate->icon; }
        if (rand()%1 < 0.5) { basic_infos.size=this->size; } else { basic_infos.size=mate->size; }
        if (rand()%1 < 0.5) { basic_infos.strength=this->strength; } else { basic_infos.strength=mate->strength; }
        if (rand()%1 < 0.5) { basic_infos.attack=this->attack; } else { basic_infos.attack=mate->attack; }
        if (rand()%1 < 0.5) { basic_infos.defense=this->defense; } else { basic_infos.defense=mate->defense; }
        if (rand()%1 < 0.5) { basic_infos.velocity=this->velocity; } else { basic_infos.velocity=mate->velocity; }
        if (rand()%1 < 0.5) { basic_infos.libido=this->libido; } else { basic_infos.libido=mate->libido; }
        if (rand()%1 < 0.5) { basic_infos.life_span=this->life_span; } else { basic_infos.life_span=mate->life_span; }
        if (rand()%1 < 0.5) { basic_infos.diet=this->diet; } else { basic_infos.diet=mate->diet; }
        Threshold threshold_infos;
        if (rand()%1 < 0.5) { threshold_infos.threshold_urgent_food=this->threshold_urgent_food; } else { threshold_infos.threshold_urgent_food=mate->threshold_urgent_food; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_urgent_water=this->threshold_urgent_water; } else { threshold_infos.threshold_urgent_water=mate->threshold_urgent_water; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_chill_food=this->threshold_chill_food; } else { threshold_infos.threshold_chill_food=mate->threshold_chill_food; }
        if (rand()%1 < 0.5) { threshold_infos.threshold_chill_water=this->threshold_chill_water; } else { threshold_infos.threshold_chill_water=mate->threshold_chill_water; }
        Genetics genetic_infos;
        if (rand()%1 < 0.5) { genetic_infos.mutation_factor=this->mutation_factor; } else { genetic_infos.mutation_factor=mate->mutation_factor; }
        Learnt learnt_infos;
        if (rand()%1 < 0.5) { learnt_infos.predators=this->predators; } else { learnt_infos.predators=mate->predators; }
        Genetic_full_data genetics = {basic_infos,threshold_infos, genetic_infos, learnt_infos};
        return genetics; 
    }
}

/**
 * Make the entity choose his next action.
 * 
 * @param distance_nearest_food
 * @param distance_nearest_water
 * @param distance_nearest_predator
 * 
 * @return Action to do (enum).
 */
ACTION Specie::choose_action(int distance_nearest_food, int distance_nearest_water, int distance_nearest_predator) {
    double food_per = food_stored / food_storage;
    double water_per = water_stored / water_storage;
    if (abs(distance_nearest_predator) <= view_distance ) {
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

/**
 * Check if the entity as enougth food and water, for example before reproducing.
 * 
 * @return Boolean if enougth food and water.
 */
bool Specie::is_chill() {
    double food_per = food_stored / food_storage;
    double water_per = water_stored / water_storage;
    if (food_per < threshold_chill_food or water_per < threshold_chill_water) {
        return false;
    } else { 
        return true;
    }
}

/**
 * Check if an other specie name is a predator.
 * 
 * @param name Name of the specie to check.
 * 
 * @return Bolean.
 */
bool Specie::is_predator(std::string name) {
    for (int i = 0; i < predators.size(); i++) {
        if (predators[i] == name) {
            return true;
        } 
    }
    return false;
}

/**
 * Make the entity attack an other.
 * 
 * @param entity Pointer to an other entity.
 */
void Specie::fight(Specie* entity) {
    consume(STRENGTH_FOOD_CONSUMPTION * strength);
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