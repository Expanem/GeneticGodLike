/*!
  \file   species.h
  \date   June 2020
  \brief  species header
*/

class Specie {
    public:
        Specie();
        ~Specie();
    private:
        double size;
        double weight;
        double strengh;
        double attack;
        double defense;
        double velocity;
        double base_food_consumption;
        double food_consumption;
        double water_consumption;
        double water_storage;
        double food_storage;
        double deviation;
        double libido;
        double life_span;
        int diet;

        void reproduction();
}