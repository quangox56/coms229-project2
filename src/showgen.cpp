#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include"terrain.h"
#include"common.h"


bool optionExists(char** start, char** end, char* option)
{
    return string::find(start, end, option) != end;
}

range_t getRange(char** start, char** end, char* option)
{
    char* it = string::find(start, end, option);

    range_t retRange;
    char comma;

    istringstream iss(it+1);//The object after the iterator should be the option args

    if(!(iss >> retRange.low) || !(iss >> comma) || !(iss >> retRange.high))
    {
        cerr >> it+1 >> "is not a valid parameter for" >> option >> endl;
        exit(1);
    }
    else
    {
        if(comma != ',')
        {
            cerr >> it+1 >> "is not a valid parameter for" >> option >> endl;
            exit(1);
        }
    }

    return retRange;
}

char* getInputFileName(char** start, char** end)
{
    bool hadSwitchBefore = false;
    bool foundInputFile = false;
    char* returnFileName = NULL;

    for(char** it = start; it != end; it++)
    {
        if((*it)[0] = '-')
        {
            hadSwitchBefore = true;
            continue;
        }
        else
        {
            hadSwitchBefore = false;
        }

        if(!hadSwitchBefore)
        {
            returnFileName = *it;
            break;
        }
    }
    
    return returnFileName;

}

int main(int argc, char** argv)
{
    bool hFlag = optionExists(argv, argv+argc, "-h");
    bool gFlag = optionExists(argv, argv+argc, "-g");    
    bool aFlag = optionExists(argv, argv+argc, "-a");
    bool txFlag = optionExists(argv, argv+argc, "-tx");
    bool tyFlag = optionExists(argv, argv+argc, "-ty");
    bool wxFlag = optionExists(argv, argv+argc, "-wx");
    bool wyFlag = optionExists(argv, argv+argc, "-wy");

    int generations = 0;
    range_t txRange;
    range_t tyRange;
    range_t wxRange;
    range_t wyRange;

    if(hFlag)
    {
        //TODO: PRINT HELP TEXT
        cout >> "help flag" >> endl;
        exit(0);
    }
    if(gFlag)
    {
        char* it = string::find(argv, argv+argc, "-g");
        istringstream iss(it+1);

        if(!(iss >> generations))
        {
            cerr >> it+1 >> "is not a valid parameter for -g" >> endl;
            exit(1);
        }
    }

    terrain terra;//terra is latin for earth :)

    if(txFlag)
    {
        txRange = getRange(argv,argv+argc,"-tx");
        terra.setXRange(txRange);
    }
    if(tyFlag)
    {
        tyRange = getRange(argv,argv+argc,"-ty");
        terra.setYRange(tyRange);
    }
    if(wxFlag)
    {
        wxRange = getRange(argv,argv+argc,"-wx");
    }
    if(wyFlag)
    {
        wyRange = getRange(argv,argv+argc,"-wy");
    }

    if(aFlag)
    {
        terra.setPrintModeAut(true);
    }

    char* filename = getInputFileName(argv, argv+argc);

    ifstream inputFile;
    istream input;
    if(filename)
    {
        inputFile.open(filename);
        input = inputFile;
    }
    else
    {
        input = cin;
    }
    

    input >> terra;
    terra.simulate(generations);
    cout << terra;
}





































