#include <iostream>
#include "vegetation.h"

/**
 * Vegetation constructor.
 * 
 * @param gspd grow speed
 * @param cnt counter
 * @param pois poisonous
 * @param top top energy
 * @param aqu aqua needs
 * @param light light needs
 * @param sta state
 * @param ico icon
 */
Vegetation::Vegetation(unsigned int gspd, unsigned int cnt, bool pois, 
               double top, double aqu, double light, STAGE sta, char ico):
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
    // std::cout << "ENERGY AT CREATION" << energy << std::endl;
}

/**
 * Vegatation destructor.
 */
Vegetation::~Vegetation()
{

}

/**
 * Update the vegetation each tick.
 * 
 * @param light
 * @param water
 */
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

/**
 * Called when the vegetation is eaten.
 * 
 * @return Stored energy.
 */
double Vegetation::eat()
{
    double ene = energy;
    // std::cout << "EATING... GAINED " << ene << "ENERGY PTS" << std::endl;
    energy = 0;
    counter = 0;
    state = damaged;
    icon = '.';
    return ene;
}

/**
 * Check if the vegetation is eatable yet.
 * 
 * @return Boolean.
 */
bool Vegetation::is_eatable(){
    if ((state == full) || (state == spreading) || (state == growing)) {
        return true;
        // std::cout << "okayfood" << std::endl;
    } else { return false; }
}