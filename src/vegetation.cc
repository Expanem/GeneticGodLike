#include <iostream>
#include "vegetation.h"

Vegetation::Vegetation()
{
    counter = 0;
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
            energy = top_energy;
            break;
        
        case full:
            state = spreading;
            break;
        
        case damaged:
            state = growing;
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
    return ene;
}