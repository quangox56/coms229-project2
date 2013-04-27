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
    void setYRange(range_t yRange);
    void setXRange(range_t xRange);
    void setWYRange(range_t wyRange);
    void setWXRange(range_t wxRange);
    void setPrintModeAut(bool _printAut);
    void simulate(int cycles);

    //Getters
    char getWindowState(int x, int y);
    color_t getWindowStateColor(int x, int y);
    string getName();
    range_t getYRange();
    range_t getXRange();
    range_t getWYRange();
    range_t getWXRange();
    
    friend istream& operator>>(istream& in, terrain& cTerrain);
    friend ostream& operator<<(ostream& out, terrain& cTerrain);


    

};
#endif
