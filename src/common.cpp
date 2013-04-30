//**********************************************************
//  common.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the code for the common functions. Check
//  common.h for function descriptions.
//
//**********************************************************
#include"common.h"

using namespace std;

void exitWithErr(string err)
{
    cerr << err << endl;
    cerr << "Program is now exiting with error." << endl;
    exit(1);
}

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

        if(!hadSwitchBefore)
        {
            returnFileName = *it;
            break;
        }
        else
        {
            hadSwitchBefore = false;
        }
    }
    
    return returnFileName;

}

