/*!
  \file   main.cc
  \date   June 2020
  \brief  main module
*/

#include <iostream>
#include <string>
#include "simulation.h"
#include "const.h"

using namespace std;

void help();

/** 
 * Main function, deal with parameters when the program is called.
 * 
 * @param argc
 * @param argv 
 * @return 0 if nor error or special exit
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        initialisation();
        main_loop(MAX_SIMULATION_ITERATIONS);
    } else {
        for (int argNb = 1; argNb < argc; argNb++) {
            if (string(argv[argNb]) == "-h" or string(argv[argNb]) == "--help") {
                help();
            } else {
                cout << string(argv[argNb]) << " is not recognized" << endl;
                exit(0);
            }
        }
    }
    return 0;
}

/**
 * Print the help and exit the program.
 */
void help() {
    cout << "This is the help. Not yet implemented. Just execute the program without parameters. " << endl;
    exit(0);
}