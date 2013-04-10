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
    bool rangesSet;
    bool isValid;
    bool printAut;




    //private member functions
    void handleKeyword(istringstream&);

    void resizeCells(size_t newSize);

    public:
    //public member functions
    terrain();
    terrain(range_t xRange, range_t yRange);
    void setRange(range_t xRange, range_t yRange);
    void setPrintModeAut(bool _printAut);
    istream& operator>>(istream& in);
    ostream& operator<<(ostream& out);


    

}
#endif
