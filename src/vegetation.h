#ifndef VEGE_H
#define VEGE_H

enum STAGE {growing,full,damaged,spreading};

class Vegetation
{
public:
    Vegetation(unsigned int gspd, unsigned int cnt, bool pois, 
               double top, double aqu, double light,STAGE sta,char ico );
    ~Vegetation();
    STAGE get_state(){return state;}
    char get_icon(){return icon;}
    double get_max_energy() {return top_energy;};
    bool is_poisonous(){return poisonous;}
    bool is_eatable();
    void update(double light,double water);
    double eat();
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
};

/*
class Grass : public Vegetation
{
    public:
    Grass(): Vegetation(2,0,false,5,0.1,0.2,growing,'*')
    {}
};
*/
#endif