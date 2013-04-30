//**********************************************************
//  langtonsAnts.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
//
//**********************************************************
#ifndef LANGTONSANTS_H
#define LANGTONSANTS_H
#include"terrain.h"

class langtonsAnts : public terrain::simulator
{
    public:
        langtonsAnts(terrain& owner);
        void simulate(int cycles);
        cell getNextState(int x, int y);
};
#endif
