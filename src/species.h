/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

class Specie {
    public:
        void newTick();
        void consuming();
        void eating(float food_quantity);
        void drinking(float water_quantity);
        void reproduction();
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
        
}