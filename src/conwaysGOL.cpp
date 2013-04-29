#include"conwaysGOL.h"
#include"common.h"
#include<vector>

conwaysGOL::conwaysGOL(terrain& owner) : simulator(owner) 
{
    name = "ConwaysLife";
    numStates = 2;
}

void conwaysGOL::simulate(int cycles)
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

cell conwaysGOL::getNextState(int x, int y)
{
    neighbors_t nearby = t.neighborInfo(x, y);

    cell returnState;

    if(t.cells[y][x] == STATE1)
    {
        if(nearby.neighbors[STATE1] == 2 || nearby.neighbors[STATE1] == 3)
        {
            returnState = STATE1;
        }
        else
        {
            returnState = STATE0;
        }
    }
    else//Cell is dead
    {
        if(nearby.neighbors[STATE1] == 3)
        {
            returnState = STATE1;
        }
        else
        {
            returnState = STATE0;
        }
    }

    return returnState;
}
