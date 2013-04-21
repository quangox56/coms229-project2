#ifndef TERRAIN_H
#define TERRAIN_H

#include<iostream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<string>
#include"common.h"


using namespace std;

class terrain
{
    private:
    //private member variables
    vector< vector<cell> > cells; //rows are y, columns are x

    int xRangeLow;
    int xRangeHigh;
    int yRangeLow;
    int yRangeHigh;
    int wxRangeLow;
    int wxRangeHigh;
    int wyRangeLow;
    int wyRangeHigh;

    vector<char> stateChars;
    vector<color_t> stateColors;

    string name;

    bool xRangeSet;
    bool yRangeSet;
    bool wxRangeSet;
    bool wyRangeSet;
    bool isValid;
    bool printAut;




    //private member functions
    void handleKeyword(istringstream&, string keyword);

    void resizeCells(size_t newSize);

    int numberOfLiveNeighbors(int x, int y);
    cell getNextState(int x, int y);
    public:
    //public member functions
    terrain();

    //Setters
    inline void setYRange(range_t yRange);
    inline void setXRange(range_t xRange);
    inline void setWYRange(range_t wyRange);
    inline void setWXRange(range_t wxRange);
    inline void setPrintModeAut(bool _printAut);
    inline void simulate(int cycles);

    //Getters
    inline char getState(int x, int y);
    inline string getName();
    inline range_t getYRange();
    inline range_t getXRange();
    inline range_t getWYRange();
    inline range_t getWXRange();
    
    friend istream& operator>>(istream& in, terrain& cTerrain);
    friend ostream& operator<<(ostream& out, terrain& cTerrain);


    

};
#endif
