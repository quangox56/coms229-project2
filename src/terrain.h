#ifndef TERRAIN_H
#define TERRAIN_H

#include<iostream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<string>
#include"common.h"



class terrain
{
    private:
    //private member variables
    vector< vector<cell> > cells; //rows are y, columns are x
    int xRangeLow;
    int xRangeHigh;
    int yRangeLow;
    int yRangeHigh;
    bool xRangeSet;
    bool yRangeSet;
    bool isValid;
    bool printAut;




    //private member functions
    void handleKeyword(istringstream&);

    void resizeCells(size_t newSize);

    int numberOfLiveNeighbors(int x, int y);
    public:
    //public member functions
    terrain();
    void setYRange(range_t yRange);
    void setXRange(range_t xRange);
    void setPrintModeAut(bool _printAut);
    void simulate(int cycles);
    istream& operator>>(istream& in);
    ostream& operator<<(ostream& out);


    

}
#endif
