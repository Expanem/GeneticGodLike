#ifndef SPECIES_H
#define SPECIES_H

#include "entities.h"
#include "tools.h"

using namespace std;

const Basics RACOON_BASIC_INFOS = {"Racoon", prey, 'R', 0.3, 10, 1, 1, 1, 0.5, 100, omnivore};
const Thresholds RACOON_THRESHOLD_INFOS = {0.25, 0.25, 0.75, 0.75};
const Coordinates RACOON_POSITION_INFOS = {0,0};
const Genetics RACOON_GENETIC_INFOS = {0.01};
const Learnt RACOON_LEARNT_INFOS = {{"Eagle"}};
const Genetic_full_data RACOON_FULL_INFOS = {RACOON_BASIC_INFOS, RACOON_THRESHOLD_INFOS, RACOON_GENETIC_INFOS, RACOON_LEARNT_INFOS};


const Basics EAGLE_BASIC_INFOS = {"Eagle", predator, 'E', 0.3, 10, 5, 2, 2, 0.5, 100, carnivore};
const Thresholds EAGLE_THRESHOLD_INFOS = {0.25, 0.25, 0.9, 0.9};
const Coordinates EAGLE_POSITION_INFOS = {0,0};
const Genetics EAGLE_GENETIC_INFOS = {0.01};
const Learnt EAGLE_LEARNT_INFOS = {{}};
const Genetic_full_data EAGLE_FULL_INFOS = {EAGLE_BASIC_INFOS, EAGLE_THRESHOLD_INFOS, EAGLE_GENETIC_INFOS, EAGLE_LEARNT_INFOS};

#endif