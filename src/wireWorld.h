//**********************************************************
//  wireWorld.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Header file for the Wire World simulator class.
//
//**********************************************************
#ifndef WIREWORLD_H
#define WIREWORLD_H
#include"terrain.h"

/* Class: wireWorld
 *
 * This class inherits from the abstract simulator that is used inside terrain. 
 * It implements the logic to simulate the wire world cellular automata.
 *
 * Friend Status: This class is a friend of Terrain so it can access it's cell data.
 */
class wireWorld : public terrain::simulator
{
    public:
        /* Function: wireWorld
         *
         * Constructor for the class. Sets the number of states and the string name.
         */
        wireWorld(terrain& owner);

        /* Function: simulate
         *
         * This function creates a tmp copy of the current vector of cells, then calculates what
         * the states should be in the next state and writes it to the cells 2d vector.
         */
        void simulate(int cycles);

        /* Function: getNextState
         *
         * This function determines the next state of the cell at coordinates (x,y) based on
         * Wire World rules.
         */
        cell getNextSate(int x, int y);
};
#endif
