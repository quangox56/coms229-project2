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
        //TODO: PRINT HELP TEXT
        cout << "help flag" << endl;
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
    QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );

    return app.exec();
}
