#include <iostream>
#include "vegetation.h"

Vegetation::Vegetation(unsigned int gspd, unsigned int cnt, bool pois, 
               double top, double aqu, double light,STAGE sta,char ico ):
grow_speed(gspd),
counter(cnt),
poisonous(pois),
top_energy(top),
aqua_needs(aqu),
light_needs(light),
state(sta),
icon(ico)
{
    energy = top_energy/2;
    std::cout << "ENERGY AT CREATION" << energy << std::endl;
}

Vegetation::~Vegetation()
{

}

void Vegetation::update(double light, double water)
{
    counter++;
    if((counter >= grow_speed) and (light >= light_needs) and (water >= aqua_needs))
    {
        switch (state)
        {
        case growing:
            state = full;
            icon = '*';
            energy = top_energy;
            counter = 0;
            break;
        
        case full:
            state = spreading;
            counter = 0;
            break;
        
        case damaged:
            state = growing;
            icon = ':';
            energy = top_energy/2;
            counter = 0;
            break;
        
        default:
            break;
        }
    }
}

double Vegetation::eat()
{
    double ene = energy;
    std::cout << "ENERGY STORED BY PLANT " << ene << std::endl;
    energy = 0;
    counter = 0;
    state = damaged;
    icon = '.';
    return ene;
}

bool Vegetation::is_eatable(){
    if (state == full or state == spreading) {
        return true;
    } else { return false; }
}