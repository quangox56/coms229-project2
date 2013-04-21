#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include <ncurses.h>
#include"terrain.h"
#include"common.h"

using namespace std;

void initDisplay()
{
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
}

int main(size_t argc, char** argv)
{
    vector<string> argvString(0);

    for(int i = 1; i < argc; i++)
    {
        argvString.push_back(argv[i]);
    }

    bool hFlag = optionExists(argvString, "-h");
    bool txFlag = optionExists(argvString, "-tx");
    bool tyFlag = optionExists(argvString, "-ty");

    if(hFlag)
    {
        //TODO: PRINT HELP TEXT
        cout << "help flag" << endl;
        exit(0);
    }

    int generations = 0;
    range_t txRange;
    range_t tyRange;
    range_t wxRange;
    range_t wyRange;

    terrain terra;//terra is latin for earth :)
    if(txFlag)
    {
        txRange = getRange(argvString,"-tx");
        terra.setXRange(txRange);
    }
    if(tyFlag)
    {
        tyRange = getRange(argvString,"-ty");
        terra.setYRange(tyRange);
    }

    string filename = getInputFileName(argvString);
    if(filename.size() != 0)
    {
        ifstream inputFile;
        inputFile.open(filename.c_str());
        inputFile >> terra;
        inputFile.close();
    }
    else
    {
        cin >> terra;
    }

    initDisplay();
    int col, row;
    getmaxyx(stdscr, col, row);
    if(col < 40 || row < 20)
    {
        exitWithErr("Terminal is too small. Minimum  column size is 40, minimum row size is 20.");
    }

    printw("Hello World!");
    refresh();
    getch();
    endwin();

    return 0;
}
