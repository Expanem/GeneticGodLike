#ifndef SPECIES_H
#define SPECIES_H

/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

class Specie {
    public:
        void newTick();
        void consume();
        void eat(float food_quantity);
        void drink(float water_quantity);
        void reproduction();
        int choose_action(float distance_nearest_food, float distance_nearest_water);
    private:
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
    private:
};

class Fighter_specie: virtual public Specie {
    public:
    private:
};

#endif