/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

#ifndef SPECIES_H
#define SPECIES_H

#include <string>
#include <vector>
#include <fstream>
#include "tools.h"
#include "vegetation.h"

enum STATE {alive, dead, disapeared};
enum DIET {herbivore, carnivore, omnivore};
enum ACTION {to_eat, to_drink, to_mate, to_flee, do_nothing};
enum SPECIE_TYPE {prey, predator};

typedef struct Basics {
    std::string name;
    SPECIE_TYPE type;
    char icon;
    double size;
    double strength;
    double attack;
    double defense;
    double velocity;
    double libido;
    double life_span;
    DIET diet;
} Basics;

typedef struct Thresholds {
    double threshold_urgent_food;
    double threshold_urgent_water;
    double threshold_chill_food;
    double threshold_chill_water;
} Threshold;

typedef struct Genetics{
    double mutation_factor;    
} Genetics;

typedef struct Learnt{
    std::vector<std::string> predators;
} Learnt;

typedef struct Genetic_full_data {
    Basics basic_infos;
    Threshold threshold_infos;
    Genetics genetic_infos;
    Learnt learnt_infos;
} Genetic_full_data;

class Specie {
    public:
        Specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos, Genetics genetic_infos, Learnt learnt_infos);
        void characteristics_update();
        void update(Coordinates nearest_food, Coordinates nearest_water, Coordinates nearest_mate, Coordinates nearest_prey, Coordinates nearest_prey_corpse, Coordinates nearest_predator, bool is_alone);
        void consume(double ratio);
        void aging();
        void eat(Vegetation* plant);
        void eat(Specie* entity);
        double be_eaten();
        void drink(double water_quantity);
        Genetic_full_data reproduction(Specie* mate);
        void move_to_objective(int distance_max = 0);
        void move_away_from_objective(int distance_max); 
        ACTION choose_action(int distance_nearest_food, int distance_nearest_water, int distance_nearest_predator); // TO DO
        void fight(Specie* entity);

        bool is_chill();
        bool is_predator(std::string name);

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

        void save(std::ofstream& flux) {
            flux << name << " " << type << " " << icon << " "
                 << size << " " << strength << " " << attack << " " << defense << " "
                 << velocity << " " << libido << " " << life_span << " " << diet << " ";
            flux << coord.x << " " << coord.y << " ";
            flux << threshold_urgent_food << " " << threshold_urgent_water << " " << threshold_chill_food << " " << threshold_chill_water << " ";
            flux << mutation_factor << " ";
            flux << predators.size() << " ";
            for (int i = 0; i < predators.size(); i++) {flux << predators[i] << " ";}
            flux << food_stored << " " << water_stored << " " << tick_lived << " " << state << " " << reproduced << " ";
            flux << std::endl;
        };
    protected:
        std::string name;
        SPECIE_TYPE type;
        char icon;
        double size;
        double mass;
        double strength;
        double attack;
        double defense;
        double velocity;
        double base_food_consumption;
        double water_consumption;
        double water_storage;
        double food_storage;
        double libido; // TO USE
        double life_span;
        DIET diet;

        int view_distance;

        double food_stored;
        double water_stored;
        double deviation; // TO USE
        int tick_lived;
        STATE state;
        int reproduced;

        std::vector<std::string> predators; // TO USE

        Coordinates coord;
        Coordinates objective;

        double velocity_storage;

        double threshold_urgent_food;
        double threshold_urgent_water;
        double threshold_chill_food;
        double threshold_chill_water;

        double mutation_factor; // TO USE
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