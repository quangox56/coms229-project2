#ifndef WIREWORLD_H
#define WIREWORLD_H
#include"terrain.h"

class wireWorld : public terrain::simulator
{
    public:
        wireWorld(terrain& owner);
        void simulate(int cycles);
        cell getNextState(int x, int y);
};
#endif
