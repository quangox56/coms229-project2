//**********************************************************
//  wireWorld.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
//
//**********************************************************
#include"wireWorld.h"
#include"common.h"
#include<vector>

wireWorld::wireWorld(terrain& owner) : simulator(owner) 
{
    name = "WireWorld";
    numStates = 4;
}

void wireWorld::simulate(int cycles)
{
    while(cycles > 0)
    {
        vector< vector<cell> > tmpCells(t.cells);
        for(int y = 0; 
            y <= (t.yRangeHigh-t.yRangeLow);
            y++)
        {
            for(int x = 0; 
                    x <= (t.xRangeHigh-t.xRangeLow);
                    x++)
            {
                if(0 <= y && y <= t.cells.size())
                {
                    if(0 <= x && x <= t.cells[y].size())
                    {
                        tmpCells[y][x] = getNextState(x,y);
                    }
                }
            }
        }
        cycles--;
        t.cells.assign(tmpCells.begin(), tmpCells.end());
    }

}

cell wireWorld::getNextState(int x, int y)
{
    neighbors_t nearby = t.neighborInfo(x, y);

    cell returnState;

    if(t.cells[y][x] == STATE0)//empty
    {
        return STATE0;
    }
    else if(t.cells[y][x] == STATE1)//electron head
    {
        returnState = STATE2;
    }
    else if(t.cells[y][x] == STATE2)//electron tail
    {
        returnState = STATE3;
    }
    else//Wire
    {
        if(nearby.neighbors[STATE1] == 1 || nearby.neighbors[STATE1] == 2)
        {
            returnState = STATE1;

        }
        else
        {
            returnState = STATE3;
        }
    }

    return returnState;
}
