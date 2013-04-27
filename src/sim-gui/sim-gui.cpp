#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QScrollArea>
#include "grid.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iterator>
#include <ncurses.h>
#include"../terrain.h"
#include"../common.h"



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


    /*
       QApplication app(argc, argv);
       QWidget *window = new QWidget;
       window->setWindowTitle("Enter Your Age");

       QSpinBox *spinBox = new QSpinBox;
       QSlider *slider = new QSlider(Qt::Horizontal);
       spinBox->setRange(0,130);
       slider->setRange(0,130);

       QObject::connect(spinBox, SIGNAL(valueChanged(int)),
       slider, SLOT(setValue(int)));
       QObject::connect(slider, SIGNAL(valueChanged(int)),
       spinBox, SLOT(setValue(int)));
       spinBox->setValue(35);

       QHBoxLayout *layout = new QHBoxLayout;
       layout->addWidget(spinBox);
       layout->addWidget(slider);
       window->setLayout(layout);
       window->show();

    //QPushButton *button = new QPushButton("Quit");
    //QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));
    //button->show();
    return app.exec();
    */


    QApplication app(argc, argv);
    //optionsDialog *dialog = new optionsDialog;
    grid *dialog = new grid(terra);

    QScrollArea scrollArea;
    scrollArea.setWidget(dialog);
    scrollArea.viewport()->setBackgroundRole(QPalette::Dark);
    scrollArea.viewport()->setAutoFillBackground(true);
    scrollArea.setWindowTitle(QObject::tr(terra.getName().c_str()));
    scrollArea.show();

    //    optionsDialog *dialog2 = new optionsDialog(dialog);
    //    dialog2->show();
    //    dialog2->activateWindow();

    return app.exec();
}
