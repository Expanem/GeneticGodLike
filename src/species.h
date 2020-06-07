/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

#ifndef SPECIES_H
#define SPECIES_H

#include <string>

typedef struct Thresholds {
    float threshold_urgent_food;
    float threshold_urgent_water;
    float threshold_chill_food;
    float threshold_chill_water;
} Threshold;

typedef struct Basics {
    std::string name;
    double size;
    double weight;
    double strengh;
    double attack;
    double defense;
    double velocity;
    double base_food_consumption;
    double base_water_consumption;
    double food_consumption;
    double water_consumption;
    double water_storage;
    double food_storage;
    double water_stored;
    double food_stored;
    double deviation;
    double libido;
    double life_span;
    int diet;
} Basics;

class Specie {
    public:
        Specie(Basics basic_infos, Thresholds threshold_infos);
        void newTick();
        void consume();
        void eat(float food_quantity);
        void drink(float water_quantity);
        void reproduction();
        int choose_action(float distance_nearest_food, float distance_nearest_water);
    protected:
        std::string name;
        double size;
        double weight;
        double strengh;
        double attack;
        double defense;
        double velocity;
        double base_food_consumption;
        double base_water_consumption;
        double food_consumption;
        double water_consumption;
        double water_storage;
        double food_storage;
        double water_stored;
        double food_stored;
        double deviation;
        double libido;
        double life_span;
        int diet;
        int tick_lived;
        bool dead;

        float threshold_urgent_food;
        float threshold_urgent_water;
        float threshold_chill_food;
        float threshold_chill_water;
};

class Pacifist_specie: virtual public Specie {
    public:
        Pacifist_specie(Basics basic_infos, Thresholds threshold_infos);
        ~Pacifist_specie();
    private:
};

class Fighter_specie: virtual public Specie {
    public:
        Fighter_specie(Basics basic_infos, Thresholds threshold_infos);
        ~Fighter_specie();
    private:
};

#endif