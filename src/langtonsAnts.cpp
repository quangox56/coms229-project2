#include"langtonsAnts.h"
#include"common.h"
#include<vector>

langtonsAnts::langtonsAnts(terrain& owner) : simulator(owner) {}

void langtonsAnts::simulate(int cycles)
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

cell langtonsAnts::getNextState(int x, int y)
{
    cell returnState;

    bool cellIsWhite;
    if(t.cells[y][x] == STATE0 ||
       t.cells[y][x] == STATE2 ||
       t.cells[y][x] == STATE3 ||
       t.cells[y][x] == STATE4 ||
       t.cells[y][x] == STATE5)
    {
        cellIsWhite = true;
    }
    else
    {
        cellIsWhite = false;
    }

    //The cell changes color if their is an ant on it
    bool cellChangesColor = false;
    if(t.cells[y][x] == STATE2 ||
       t.cells[y][x] == STATE3 ||
       t.cells[y][x] == STATE4 ||
       t.cells[y][x] == STATE5 ||
       t.cells[y][x] == STATE6 ||
       t.cells[y][x] == STATE7 ||
       t.cells[y][x] == STATE8 ||
       t.cells[y][x] == STATE9)
    {
        cellChangesColor = true;
    }

    if(cellChangesColor)
    {
        cellIsWhite = !cellIsWhite;
    }
    
    bool antEntering = false;
    bool twoAntsEntering = false;
    if(0 <= y-1 && y-1 <= (t.yRangeHigh-t.yRangeLow))
    {
        if(t.cells[y-1][x] == STATE2 || 
                t.cells[y-1][x] == STATE6)
        {
            antEntering = true;
            if(cellIsWhite)
            {
                //Turn east
                returnState = STATE3;
            }
            else
            {
                //Turn west
                returnState = STATE9;
            }
        }
    }
    
    if(0 <= y+1 && y+1 <= (t.yRangeHigh-t.yRangeLow))
    {
        if(t.cells[y+1][x] == STATE4 || 
                t.cells[y+1][x] == STATE8)
        {
            if(antEntering)
            {
                twoAntsEntering = true;
            }
            antEntering = true;
            
            if(cellIsWhite)
            {
                //Turn West
                returnState = STATE5;
            }
            else
            {
                //Turn east
                returnState = STATE7;
            }
        }
    }

    if(0 <= x-1 && x-1 <= (t.xRangeHigh-t.xRangeLow))
    {
        if(t.cells[y][x-1] == STATE3 || 
                t.cells[y][x-1] == STATE7)
        {
            if(antEntering)
            {
                twoAntsEntering = true;
            }
            antEntering = true;
            if(cellIsWhite)
            {
                //Turn West
                returnState = STATE4;
            }
            else
            {
                //Turn east
                returnState = STATE6;
            }
        }
    }

//  N
//W   E
//  S
    if(0 <= x+1 && x+1 <= (t.xRangeHigh-t.xRangeLow))
    {
        if(t.cells[y][x+1] == STATE9 || 
                t.cells[y][x+1] == STATE5)
        {
            if(antEntering)
            {
                twoAntsEntering = true;
            }
            antEntering = true;
            if(cellIsWhite)
            {
                //Turn West
                returnState = STATE2;
            }
            else
            {
                //Turn east
                returnState = STATE8;
            }
        }
    }

    //if there are two or more ants entering they destroy eachother
    //or if there are no ants entering the tile is empty
    if(twoAntsEntering || !antEntering)
    {
        if(cellIsWhite)
        {
            returnState = STATE0;
        }
        else
        {
            returnState = STATE1;
        }
    }

    return returnState;
}
