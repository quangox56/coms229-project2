//**********************************************************
//  terrain.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  This contains the class definiton for the terrain class
//  which is where the main logic of this project gets declared.
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

/* Class: terrain
 *
 *  The terrain class contains the grid information
 *  for the cellular automata. It contains all the functions
 *  related to processing it, and contains an abstract 
 *  simulator class that is used for implementing
 *  different rule sets.
 *
 *  The terrain class has overloaded extraction and insertion operators
 *  that load information from an .aut file and can print either an .aut or
 *  a grid depending on the print mode.
 */
class terrain
{
    private:
        //private member variables
        
        //This is the main data of this class. Everything else functions
        //to either assist in manipulating these cells and printing their
        //data.
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

        /* Function: handleKeyword
         *
         * This helper function assists in parsing the .aut files by
         * parsing the legitimate keywords.
         */
        void handleKeyword(istringstream&, string keyword);

        /* Function: resizeCells
         *
         * This function is used when it is necessary to grow the size of the 2d
         *  cells array.
         *
         *  Note: despite it's name this function will never decrease the current
         *  size of th vectors.
         */
        void resizeCells(size_t newSize);

        
        /* Function: neighborInfo
         *
         * neighborInfo is used to get a count of all cells
         *  in a neighborhood around a cell and what their 
         *  states are.
         */
        neighbors_t neighborInfo(int x, int y);
    public:
        //public member functions

        /* Function: terrain
         *
         * Constructor for the class. Initializes all the
         *  built in defaults.
         */
        terrain();
        /* Function: ~terrain
         *
         * Destructor ensures that the memory allocated for the 
         *  simulator class is destroyed upon program termination.
         */
        ~terrain();

        //Setters
        /* Function: setYRange
         *
         * This function sets the terrain Y range and overrules any
         *  Y range read from the aut file.
         */
        void setYRange(range_t yRange);
        /* Function: setXRange
         *
         * This function sets the terrain X range and overrules any
         *  X range read from the aut file.
         */
        void setXRange(range_t xRange);
        /* Function: setWYRange
         *
         * This function sets the window Y range.
         */
        void setWYRange(range_t wyRange);
        /* Function: setWXRange
         *
         * This function sets the window X range.
         */
        void setWXRange(range_t wxRange);
        /* Function: setPrintModeAut
         *
         * This function will change whether or
         *  not the class prints an aut file or a simple
         *  grid as output.
         */
        void setPrintModeAut(bool _printAut);
        /* Function: simulate
         *
         * This function will call the simulator classes simulate function
         *  if the terrain class is in a valid state.
         */
        void simulate(int cycles);

        //Getters

        /* Function: getWindowState
         *
         * This function is used by the TUI/GUI to get the char state of a cell
         *  relative to it's window coordinates.
         */
        char getWindowState(int x, int y);
        /* Function: getWindowStateColor
         *
         * This function is used by the TUI/GUI to get the color state of a cell
         *  relative to it's window coordinates.
         */
        color_t getWindowStateColor(int x, int y);
        /* Function: getName
         *
         * This function returns the name of the terrain if specified in the .aut
         */
        string getName();
        /* Function: getYRange
         *
         * This function returns the terrain Y range.
         */
        range_t getYRange();
        /* Function: getXRange
         *
         * This function returns the terrain X range.
         */
        range_t getXRange();
        /* Function: getWYRange
         *
         * This function returns the window Y Range.
         */
        range_t getWYRange();
        /* Function: getWXRange
         *
         * This function returns the window X range.
         */
        range_t getWXRange();

        /* Function: operator>>
         *
         * This function is used to read in a .aut file. It works
         *  by first reading the file into seperate lines, then 
         *  removing all comments. It then splits everything into statements
         *  which are deliminated by semicolons, and finds the statements that 
         *  begin with valid keywords. After locating a statement that begins
         *  with a valid keyword it will pass that statement to the helper 
         *  function handleKeyword which will finish the parsing for that 
         *  particular keyword.
         */
        friend istream& operator>>(istream& in, terrain& cTerrain);
        /* Function: operator<<
         *
         * This function is used to output the terrain. If the terrain is set to print
         * aut mode, then it will output a valid aut file, else it will print it as a grid
         * of chars using the chars specified in the .aut.
         */
        friend ostream& operator<<(ostream& out, terrain& cTerrain);

        /* Class: simulator
         *
         * This abstract class is used to allow the terrain to swap out it's
         *  simulation rules at runtime, depending on what the .aut file
         *  specifies. This allows a new set of rules to be easily implemented by
         *  simply creating a new simulator class.
         */
        class simulator
        {
            public:
                string name;
                int numStates;
                /* Function: simulator
                 *
                 * constructor, sets a reference to the terrain so
                 *  the derived classes have easy access to the cells
                 *  for manipulation.
                 */
                simulator(terrain& owner);
                /* Function: simulate
                 *
                 * Abstract function that the derived classes must
                 *  implement. This is where the different rules
                 *  would be implemented.
                 */
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
