//**********************************************************
//  sim-gui.cpp
//  
//  Author: Ryan Scheel (rascheel@iastate.edu)
//
//  Contains the main function for the sim-gui. This sim-gui
//  launches the grid GUI components with the required parameters.
//  This code also handles all required argument processing.
//
//**********************************************************
#include <QApplication>
#include <QScrollArea>
#include "grid.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include"../terrain.h"
#include"../common.h"
#include"../conwaysGOL.h"
#include"../wireWorld.h"
#include"../briansBrain.h"
#include"../langtonsAnts.h"



/* Class: main
 *
 *  This launches the grid GUI components with the required parameters.
 *  This code also handles all required argument processing.
 */
int main(int argc, char *argv[])
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
        string help = "simgui\n\n"
                      "This program reads a single .aut file, either from a pathname\n"
                      "passed as an argument, or from standard input if no file names\n"
                      "given as arguments. Then provides a gui for simulating.\n\n"
                      "Allowable switches:\n"
                      "\t-h     \t\tdisplay this help screen.\n"
                      "\t-tx l,h\t\tset the x range of the terrain, overrides .aut.\n"
                      "\t-ty l,h\t\tset the y range of the terrain, overrides .aut.\n";
        cout << help << endl;
        exit(0);
    }

    range_t txRange;
    range_t tyRange;

    terrain terra;
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


    QApplication app(argc, argv);
    grid *dialog = new grid(terra);

    QScrollArea scrollArea;
    scrollArea.setWidget(dialog);
    scrollArea.viewport()->setBackgroundRole(QPalette::Dark);
    scrollArea.viewport()->setAutoFillBackground(true);
    scrollArea.setWindowTitle(QObject::tr(terra.getName().c_str()));
    scrollArea.show();

    return app.exec();
}
