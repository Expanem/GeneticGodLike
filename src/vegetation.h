#ifndef VEGE_H
#define VEGE_H

enum STAGE {growing,full,damaged,spreading};

class Vegetation
{
protected:
    STAGE state;
    char icon;
    bool poisonous;
    double energy;
    unsigned int grow_speed;
    unsigned int counter;
    double top_energy;
    double aqua_needs;
    double light_needs;
public:
    Vegetation();
    ~Vegetation();
    STAGE get_state(){return state;}
    char get_icon(){return icon;}
    bool is_poisonous(){return poisonous;}
    void update(double light,double water);
    double eat();

};

class Grass : public Vegetation
{
    public:
    Grass(){
        grow_speed = 2;
        counter = 0;
        poisonous = false;
        top_energy = 5;
        aqua_needs = 0.1;
        light_needs = 0.2;
        state = growing;
        icon = '*';
    }
};

#endif