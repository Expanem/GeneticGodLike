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

typedef struct Positions {
    int x_position;
    int y_position;
} Positions;

typedef struct Basics {
    std::string name;
    char icone;
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
        Specie(Basics basic_infos, Positions position_info, Thresholds threshold_infos);
        void newTick();
        void consume();
        void eat(float food_quantity);
        void drink(float water_quantity);
        void reproduction();
        void move_to_objective();
        int choose_action(float distance_nearest_food, float distance_nearest_water);

        std::string get_name(){return name;};
        char get_icone(){return icone;};
    protected:
        std::string name;
        char icone;
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

        int x_position;
        int y_position;

        int x_objective;
        int y_objective;
        float velocity_storage;

        float threshold_urgent_food;
        float threshold_urgent_water;
        float threshold_chill_food;
        float threshold_chill_water;
};

class Pacifist_specie: virtual public Specie {
    public:
        Pacifist_specie(Basics basic_infos, Positions position_info, Thresholds threshold_infos);
        ~Pacifist_specie();
    private:
};

class Fighter_specie: virtual public Specie {
    public:
        Fighter_specie(Basics basic_infos, Positions position_info, Thresholds threshold_infos);
        ~Fighter_specie();
    private:
};

#endif