//**********************************************************
//  briansBrain.cpp
//    
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the code for the brians brain simulator class.
//  Check briansBrain.h for function comments.
//
//**********************************************************
#include"briansBrain.h"
#include"common.h"
#include<vector>

briansBrain::briansBrain(terrain& owner) : simulator(owner) 
{
    name = "BriansBrain";
    numStates = 3;
}

void briansBrain::simulate(int cycles)
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

cell briansBrain::getNextState(int x, int y)
{
    neighbors_t nearby = t.neighborInfo(x, y);

    cell returnState;

    if(t.cells[y][x] == STATE0)
    {
        if(nearby.neighbors[STATE2] == 2)
        {
            returnState = STATE2;
        }
        else
        {
            returnState = STATE0;
        }
    }
    else if(t.cells[y][x] == STATE1)
    {
        returnState = STATE0;
    }
    else//Cell is on
    {
        returnState = STATE1;
    }

    return returnState;
}
