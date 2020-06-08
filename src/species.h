/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

#ifndef SPECIES_H
#define SPECIES_H

#include <string>
#include "tools.h"

typedef struct Thresholds {
    float threshold_urgent_food;
    float threshold_urgent_water;
    float threshold_chill_food;
    float threshold_chill_water;
} Threshold;

typedef struct Basics {
    std::string name;
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
    int diet;
} Basics;

class Specie {
    public:
        Specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        void newTick(int action, Coordinates objective);
        void consume(float ratio);
        void eat(float food_quantity);
        void drink(float water_quantity);
        void reproduction();
        void move_to_objective();
        int choose_action(float distance_nearest_food, float distance_nearest_water);

        std::string get_name(){return name;};
        char get_icon(){return icon;};
        Coordinates get_coordinates(){return coord;}; 
        bool get_state(){return dead;};

        void set_icon(char ic){icon = ic;};
    protected:
        std::string name;
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
        int diet;

        double food_stored;
        double water_stored;
        double deviation;
        int tick_lived;
        bool dead;

        Coordinates coord;
        Coordinates objective;

        float velocity_storage;

        float threshold_urgent_food;
        float threshold_urgent_water;
        float threshold_chill_food;
        float threshold_chill_water;
};

class Pacifist_specie: virtual public Specie {
    public:
        Pacifist_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        ~Pacifist_specie();
    private:
};

class Fighter_specie: virtual public Specie {
    public:
        Fighter_specie(Basics basic_infos, Coordinates position_info, Thresholds threshold_infos);
        ~Fighter_specie();
    private:
};

#endif