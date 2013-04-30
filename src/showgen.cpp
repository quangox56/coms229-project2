//**********************************************************
//  showgen.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the main function for showgen. Instantiates the
//  terrain class and uses it as necessary to simulate.
//  This function also handles all required argument processing.
//
//**********************************************************
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include<vector>
#include"terrain.h"
#include"common.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<string> argvString(0);

    for(int i = 1; i < argc; i++)
    {
        argvString.push_back(argv[i]);
    }

    bool hFlag = optionExists(argvString, "-h");
    bool gFlag = optionExists(argvString, "-g");    
    bool aFlag = optionExists(argvString, "-a");
    bool txFlag = optionExists(argvString, "-tx");
    bool tyFlag = optionExists(argvString, "-ty");
    bool wxFlag = optionExists(argvString, "-wx");
    bool wyFlag = optionExists(argvString, "-wy");

    int generations = 0;
    range_t txRange;
    range_t tyRange;
    range_t wxRange;
    range_t wyRange;

    if(hFlag)
    {
        //TODO: PRINT HELP TEXT
        cout << "help flag" << endl;
        exit(0);
    }
    if(gFlag)
    {
        vector<string>::iterator it = find(argvString.begin(), argvString.end(), "-g");
        istringstream iss(*(it+1));

        if(!(iss >> generations))
        {
            cerr << *(it+1) << "is not a valid parameter for -g" << endl;
            cerr << "Program will now exit with error." << endl;
            exit(1);
        }
    }

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
    if(wxFlag)
    {
        wxRange = getRange(argvString,"-wx");
        terra.setWXRange(wxRange);
    }
    if(wyFlag)
    {
        wyRange = getRange(argvString,"-wy");
        terra.setWYRange(wyRange);
    }

    if(aFlag)
    {
        terra.setPrintModeAut(true);
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
    
    terra.simulate(generations);
    cout << terra;
}

