/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

#ifndef SPECIES_H
#define SPECIES_H

#include <string>
#include "tools.h"
#include "vegetation.h"

enum STATE {alive, dead, disapeared};
enum DIET {herbivore, carnivore, omnivore};
enum ACTION {to_eat, to_drink, to_mate, to_flee, do_nothing};
enum SPECIE_TYPE {prey, predator};

typedef struct Thresholds {
    float threshold_urgent_food;
    float threshold_urgent_water;
    float threshold_chill_food;
    float threshold_chill_water;
} Threshold;

typedef struct Basics {
    std::string name;
    SPECIE_TYPE type;
    char icon;
    double size;
    double weight;
    double strengh;
    double attack;
    double defense;
    double velocity;
    double food_consumption;
    double water_consumption;
    double water_storage;
    double food_storage;
    double libido;
    double life_span;
    DIET diet;
} Basics;

typedef struct Genetic_full_data {
    Basics basic_infos;
    Threshold threshold_infos;
} Genetic_full_data;

class Specie {
    public:
        Specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        void update(Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey, Coordinates nearest_prey_corpse, Coordinates nearest_predator, bool is_alone);
        void consume(float ratio);
        void eat(Vegetation* plant);
        void eat(Specie* entity);
        double be_eaten();
        void drink(float water_quantity);
        Genetic_full_data reproduction(Specie* mate);
        void move_to_objective(int distance_max = 0);
        void move_away_from_objective(int distance_max = 5); // CHOOSE BETTER, USE CONST
        ACTION choose_action(int distance_nearest_food, int distance_nearest_water, int distance_nearest_predator); // TO DO
        void fight(Specie* entity);

        bool is_chill();

        std::string get_name(){return name;};
        char get_icon(){return icon;};
        Coordinates get_coordinates(){return coord;}; 
        STATE get_state(){return state;};
        double get_food_stored(){return food_stored;};
        double get_water_stored(){return water_stored;};
        int get_reproduced(){return reproduced;};
        double get_attack(){return attack;};
        double get_defense(){return defense;};
        DIET get_diet(){return diet;};
        SPECIE_TYPE get_type(){return type;};

        void reset_reproduced(){reproduced = 0;};
        void increase_reproduced(){reproduced++;};
        void set_icon(char ic){icon = ic;};
        void set_state(STATE new_state){state = new_state;};
    protected:
        std::string name;
        SPECIE_TYPE type;
        char icon;
        double size;
        double weight;
        double strengh;
        double attack;
        double defense;
        double velocity;
        double food_consumption;
        double water_consumption;
        double water_storage;
        double food_storage;
        double libido;
        double life_span;
        DIET diet;

        double food_stored;
        double water_stored;
        double deviation;
        int tick_lived;
        STATE state;
        int reproduced;

        Coordinates coord;
        Coordinates objective;

        float velocity_storage;

        float threshold_urgent_food;
        float threshold_urgent_water;
        float threshold_chill_food;
        float threshold_chill_water;
};
/*
class Pacifist_specie: virtual public Specie {
    public:
        Pacifist_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        ~Pacifist_specie();
        void fight(Specie* entity);
    private:
};

class Fighter_specie: virtual public Specie {
    public:
        Fighter_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        ~Fighter_specie();
        void fight(Specie* entity);
    private:
}; 
*/

#endif