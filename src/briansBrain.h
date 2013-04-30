//**********************************************************
//  briansBrain.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
//
//**********************************************************
#ifndef BRIANSBRAIN_H
#define BRIANSBRAIN_H
#include"terrain.h"

class briansBrain : public terrain::simulator
{
    public:
        briansBrain(terrain& owner);
        void simulate(int cycles);
        cell getNextState(int x, int y);
};
#endif
