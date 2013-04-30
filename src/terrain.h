//**********************************************************
//  terrain.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
//
//**********************************************************
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

        bool keywordsFound[NUM_KEYWORDS];
        bool xRangeSet;
        bool yRangeSet;
        bool wxRangeSet;
        bool wyRangeSet;
        bool isValid;
        bool printAut;



        //private member functions
        void handleKeyword(istringstream&, string keyword);

        void resizeCells(size_t newSize);

        neighbors_t neighborInfo(int x, int y);
    public:
        //public member functions
        terrain();
        ~terrain();

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

        class simulator
        {
            public:
                string name;
                int numStates;
                simulator(terrain& owner);
                virtual void simulate(int cycles) = 0;
            protected:
                terrain &t;

        };

        friend class simulator;
        friend class conwaysGOL;
        friend class langtonsAnts;
        friend class wireWorld;
        friend class briansBrain;

    private:
        simulator* sim;



};
#endif
