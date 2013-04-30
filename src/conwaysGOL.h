//**********************************************************
//  conwaysGOL.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
//
//**********************************************************
#ifndef CONWAYSGOL_H
#define CONWAYSGOL_H
#include"terrain.h"

class conwaysGOL : public terrain::simulator
{
    public:
        conwaysGOL(terrain& owner);
        void simulate(int cycles);
        cell getNextState(int x, int y);
};
#endif
