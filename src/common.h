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

#define NUM_KEYWORDS 6
#define REQ_KEYWORDS 3 //Only Xrange, Yrange, and Initial are required in an aut
#define KEYWORDS {"Xrange", "Yrange", "Initial", "Name", "Chars", "Colors"}

enum cell {DEAD, ALIVE};

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

void exitWithErr(std::string error);
bool optionExists(std::vector<std::string> argStrings, std::string option);
range_t getRange(std::vector<std::string> argStrings, std::string option);
std::string getInputFileName(std::vector<std::string> argStrings);

#endif
