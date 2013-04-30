//**********************************************************
//  common.h
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  FILE DESCRIPTION
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

#define NUM_KEYWORDS 7
#define REQ_KEYWORDS 3 //Only Xrange, Yrange, and Initial are required in an aut
#define KEYWORDS {"Xrange", "Yrange", "Initial", "Name", "Chars", "Colors", "Rules"}

enum cell {STATE0, STATE1, STATE2, STATE3, STATE4, STATE5, STATE6, STATE7, STATE8, STATE9};

struct range_t
{
    int low;
    int high;
};

struct color_t
{
    int r;
    int g;
    int b;
};

struct neighbors_t
{
    //This array is indexed by the cell type to convey number of neighbors
    //eg: number of neighbors that are in state 0 is stored in neighbor[STATE0]
    int neighbors[10];
};

void exitWithErr(std::string error);
bool optionExists(std::vector<std::string> argStrings, std::string option);
range_t getRange(std::vector<std::string> argStrings, std::string option);
std::string getInputFileName(std::vector<std::string> argStrings);

#endif
