//**********************************************************
//  common.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  This header contains definitions for common data types 
//  used across all programs and declaraions of useful functions
//  that are commonly used.
//
//  It also declares some constants that are used in various 
//  places.
//
//**********************************************************
#ifndef COMMON_H
#define COMMON_H
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include<vector>

//  These constants are used to easily add new keywords without making too many changes
//  to existing code.
#define NUM_KEYWORDS 7
#define REQ_KEYWORDS 3 //Only Xrange, Yrange, and Initial are required in an aut
#define KEYWORDS {"Xrange", "Yrange", "Initial", "Name", "Chars", "Colors", "Rules"}

//  This enum displays the possible states of a cell for code readability purposes.
enum cell {STATE0, STATE1, STATE2, STATE3, STATE4, STATE5, STATE6, STATE7, STATE8, STATE9};

//  This struct declares a high and low range, useful for minimizing arguments to functions
//  and simplifying return types.
struct range_t
{
    int low;
    int high;
};

//  This struct defines the information for one RGB color.
struct color_t
{
    int r;
    int g;
    int b;
};

//  Contains the count of a cell's neighbors and their states to simplify
//  return types.
struct neighbors_t
{
    //This array is indexed by the cell type to convey number of neighbors
    //eg: number of neighbors that are in state 0 is stored in neighbor[STATE0]
    int neighbors[10];
};

/* Function: exitWithErr
 *
 *  This function is the major error handling in the program. When an error is 
 *  encountered this function is called with a string describing the error. 
 *  Then the function will print that the program is exiting in error and exit.
 */
void exitWithErr(std::string error);

/* Function: optionExists
 *
 *  This function is used for parsing arguments and determing if a particualr
 *  option exists among them.
 */
bool optionExists(std::vector<std::string> argStrings, std::string option);

/* Function: getRange
 *
 *  This function is used in conjunction with the -tx style arguments to parse
 *  the high and low ranges that are comma seperated.
 */
range_t getRange(std::vector<std::string> argStrings, std::string option);

/* Function: getInputFileName
 *
 *  This function is used in argument parsing to determine which of all the
 *  arguments passed in is apossible filename.
 */
std::string getInputFileName(std::vector<std::string> argStrings);

#endif
