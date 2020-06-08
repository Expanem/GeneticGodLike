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
            break;
        
        case full:
            state = spreading;
            break;
        
        case damaged:
            state = growing;
            icon = ':';
            energy = top_energy/2;
            break;
        
        default:
            break;
        }
    }
}

double Vegetation::eat()
{
    double ene = energy;
    energy = 0;
    state = damaged;
    icon = '.';
    return ene;
}

bool Vegetation::is_eatable(){
    if (state == full or state == spreading) {
        return true;
    } else { return false; }
}