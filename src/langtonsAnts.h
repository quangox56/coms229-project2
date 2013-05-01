//**********************************************************
//  langtonsAnts.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Header file for the Langton's Ant simulator class.
//
//**********************************************************
#ifndef LANGTONSANTS_H
#define LANGTONSANTS_H
#include"terrain.h"

/* Class: langtonsAnts
 *
 * This class inherits from the abstract simulator that is used inside terrain. 
 * It implements the logic to simulate lanton's ants cellular automata.
 *
 * Friend Status: This class is a friend of Terrain so it can access it's cell data.
 */
class langtonsAnts : public terrain::simulator
{
    public:
        /* Function: langtonsAnts
         *
         * Constructor for the class. Sets the number of states and the string name.
         */
        langtonsAnts(terrain& owner);

        /* Function: simulate
         *
         * This function creates a tmp copy of the current vector of cells, then calculates what
         * the states should be in the next state and writes it to the cells 2d vector.
         */
        void simulate(int cycles);

        /* Function: getNextState
         *
         * This function determines the next state of the cell at coordinates (x,y) based on
         * Langton's Ants rules.
         */
        cell getNextState(int x, int y);
};
#endif
