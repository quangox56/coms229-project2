#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include"terrain.h"
#include"common.h"

using namespace std;

bool optionExists(vector<string> argStrings, string option)
{
    return find(argStrings.begin(), argStrings.end(), option) != argStrings.end();
}

range_t getRange(vector<string> argStrings, string option)
{
    vector<string>::iterator it = find(argStrings.begin(), argStrings.end(), option);

    range_t retRange;
    char comma;

    istringstream iss(*(it+1));//The object after the iterator should be the option args

    if(!(iss >> retRange.low) || !(iss >> comma) || !(iss >> retRange.high))
    {
        cerr << *(it+1) << "is not a valid parameter for" << option << endl;
        exit(1);
    }
    else
    {
        if(comma != ',')
        {
            cerr << *(it+1) << "is not a valid parameter for" << option << endl;
            exit(1);
        }
    }

    return retRange;
}

string getInputFileName(vector<string> argStrings)
{
    bool hadSwitchBefore = false;
    bool foundInputFile = false;
    string returnFileName;

    for(vector<string>::iterator it = argStrings.begin(); it != argStrings.end(); it++)
    {
        if((*it) == "-g" ||
           (*it) == "-a" ||
           (*it) == "-tx" ||
           (*it) == "-ty" ||
           (*it) == "-wy" ||
           (*it) == "-wx")
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

